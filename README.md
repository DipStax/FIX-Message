# FIX Message Serialization Library

## Overview

This C++20 library provides a type-safe, extensible framework for **serializing and deserializing FIX protocol messages**. It uses modern C++ features (concepts, templates, and meta-programming) to represent FIX tags, message types, and message structures, enabling compile-time validation and efficient runtime parsing.

## Features

- **Type-safe FIX tags**: Each tag is represented as a strongly-typed object.
- **Message composition**: Compose messages from tag lists and tag groups.
- **Serialization/Deserialization**: Convert between FIX string format and C++ objects.
- **Optional and required tags**: Supports both required and optional tags.
- **Custom tag value conversion**: Easily extendable for custom types.
- **Exception handling**: Robust error reporting for invalid or out-of-range values.
- **Unit tests**: Includes GoogleTest-based tests for tag parsing and validation.

## Header

The header is composed of 2 types of field: **positional** and **non positional**.
The positional field are parse in order and encaplusated over `fix::PositionaTag`, if a field contain in the positional wrapper is not meet before the insertion of a **non positional** field a `fix::RejectError` is return.
All the **positional** field must be required, but **non positional** can be optional.

```cpp
constexpr const fix::TagName BeginString = 8;
constexpr const fix::TagName BodyLength = 9;
constexpr const fix::TagName MsgType = 35;
constexpr const fix::TagName SenderCompID = 49;
constexpr const fix::TagName TargetCompID = 56;
constexpr const fix::TagName DeliverToCompID = 128;

using Header = fix::Header<
    // positional tag declaration
    fix::PositionalTag<
        fix::Tag<BeginString, std::string>,                 // required field
        fix::Tag<BodyLength, int>,
        fix::Tag<MsgType, std::string>
    >,
    // non positional tag declaration
    fix::Tag<SenderCompID, std::string>,
    fix::Tag<TargetCompID, std::string>,
    fix::Tag<DeliverToCompID, std::optional<std::string>>   // Optional field
>;
```

**Handling error:**

You can call the function `verify` to check if all the **positional** tag are present. For the **non positional** the library doesn't implement a verification system for duplicate or missing and should be handle by the user during parsing.

To insert a field value into the header use the `try_insert` member function.
It return `true` as a value from the `Expected`, if the field is part of the header otherwise `false`, this allow to know when the header end and parse the remaining of message later.
If an error occured like a unknow tag the error message will be available as "unexpected".

```cpp
constexpr const fix::TagName MessageSpecificTag = 1;

Header header{};
xstd::Expected<bool, fix::RejectError> reject{false};

reject = header.try_insert(BeginString, "FIX4.2");          // reject.has_value() == true & reject.value() == true
reject = header.try_insert(BodyLength, "123");              // ...
reject = header.try_insert(MsgType, "E");
reject = header.try_insert(MessageSpecificTag, "value");    // reject.has_value() == true & reject.value() == false
```

**Design note:**
The design choice to insert elements one by one (using `try_insert`) is intentional. It allows parsing to occur incrementally as data arrives from the socket, which is typical in high-performance FIX engines. This avoids buffering the entire message before parsing, enabling immediate validation and processing of each field. The approach is to validate the header first, then parse at once the body using `BodyLength` field.

## Message example usage

> This example will partally implement the [New Order - List](https://www.onixs.biz/fix-dictionary/4.2/msgType_E_69.html) message

A `fix::Message` is the declaration of the body of a message, with support of list and regular field.
To declare a list of field you use the `fix::List` class encapsulated inside a `fix::TagList`, wich will contain all the list available in the message.
Inside the list you need to declare a `fix::TagRef` who is the **No** field of the message, it's second argument specify if the list is optional (`true` mean optional, `false` required).

```cpp
// declare message type
namespace fix42
{
    constexpr const char NewOrderList = 'E';
}

// declare field
constexpr const fix::TagName OrderQty = 38;
constexpr const fix::TagName Symbol = 55;
constexpr const fix::TagName ListId = 66;
constexpr const fix::TagName NoOrders = 73;
constexpr const fix::TagName BidType = 394;

// Compose a message type
using NewOrderList = fix::Message<fix42::NewOrderList,
    fix::TagList<
        // declare the list
        fix::List<
            fix::TagRef<NoOrders, false>                // No field required
            fix::Tag<Symbol, std::string>,
            fix::Tag<OrderQty, std::optional<float>>
            // ...
        >
        // ...
    >,
    fix::Tag<ListId, std::string>,
    fix::Tag<BidType, int>
    // ...
>;
```

To parse the message you need to provide a `std::vector<std::pair<std::string, std::string>>` that will represente field/value of the message. The reason bind the use of `std::vector` instead of `std::map`, is to keep the order of field, because the engine required the **No** Field to be followed by the item's list:

```cpp
// Parse a FIX message string
NewOrderList msg;
std::optional<fix::RejectError> error = msg.from_string({
    { "66", "list_id" },
    { "73", "2" },          /* No Field */
    { "55", "USD/EUR" },    /* list 1 */
    { "38", "3520.15" },    /* list 1 */
    { "55", "YEN/EUR" },    /* list 2 */
    { "38", "524.36" },     /* list 2 */
    { "394", "1" }
});

if (error.has_value()) {
    error.value();              // handle the error
}
```

To access regular field (not contained in list) you can directly use the `get<fix::TagName>` method of the message:

```cpp
// Access regular tag
std::cout << "List ID: " << msg.get<ListId>().Value << "\n";
std::cout << "Bid ID: " << msg.get<BidType>().Value << "\n";
```

And to access the field inside of a list you can either use **iteration** or `at` accessor:

```cpp
// using iterator
for (const auto &_tuple : msg.getList<NoOrder>()) {
    std::cout << "\tSymbol: " << fix::get<Symbol>(_tuple).Value << "\n";
    if (fix::get<Symbol>(_tuple).Value.has_value())
        std::cout << "\tOrder Qty: " << fix::get<OrderQty>(_tuple).Value << "\n";
}

// using accessor
const auto &list = msg.getList<NoOrder>();

for (size_t it = 0; it < list.TagNo.Value; it++) {
    const auto &tuple = list.at(it);

    std::cout << "\tSymbol: " << fix::get<Symbol>(tuple).Value << "\n";
    if (fix::get<Symbol>(tuple).Value.has_value())
        std::cout << "\tOrder Qty: " << fix::get<OrderQty>(tuple).Value << "\n";
}
```

To serialize the message to a valid FIX string simply use `to_string` member function:

```cpp
std::cout << msg.to_string() << std::endl;
```

## Tag Value Conversion

For each type use in a message you need to have function `to_FIX` that take a as parameter a `std::string` and a reference to the type you wan't to implement. This function will return an optional `RejectError` in case the conversion went wrong.

```cpp
std::optional<fix::RejectError> from_FIX(const std::string &_value, int &_out);
```

You also need a function `from_FIX` that will be use to convert to string the content of the `Header`/`Message`, using as parameter a reference to a `std::string` and then the type you wan't to convert.

```cpp
void to_FIX(std::string &_out, int _value);
```

> The library allow the use of reference on the `_value` (usefull for example in the case of `std::string`).

## Building & Testing

1. **Build the library and tests:**

```sh
mkdir build && cd build
cmake ..
make
```

2. **Run unit tests:**

```sh
./fix_message_test
```

## Requirements

- C++20 compiler (tested with GCC)
- CMake 3.16+
- GoogleTest (automatically fetched by CMake)

---

**This library is ideal for building FIX engines, routers, and test tools with strong type safety and extensibility.**