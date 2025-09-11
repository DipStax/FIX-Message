#pragma once

#include <variant>

namespace xstd
{
    template<class Error>
    class Unexpected
    {
        public:
            explicit Unexpected(const Error &err);
            explicit Unexpected(Error &&err);

            const Error &error() const;
            Error &error();

        private:
            Error m_error;
    };

    template<class Result, class Error>
    class Expected
    {
        public:
            Expected(const Result &result);
            Expected(Result &&result);

            Expected(const Unexpected<Error> &error);
            Expected(Unexpected<Error> &&error);

            [[nodiscard]] bool has_value() const;
            [[nodiscard]] bool has_error() const;

            explicit operator bool() const;

            const Result &value() const;
            Result &value();

            const Error &error() const;
            Error &error();

        private:
            std::variant<Result, Error> m_data;
    };
}

#include "Expected.inl"