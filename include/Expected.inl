#include "Expected.hpp"

namespace xstd
{
    template<class Error>
    Unexpected<Error>::Unexpected(const Error& err)
        : m_error(err)
    {
    }

    template<class Error>
    Unexpected<Error>::Unexpected(Error&& err)
        : m_error(std::move(err))
    {
    }

    template<class Error>
    const Error &Unexpected<Error>::error() const
    {
        return m_error;
    }

    template<class Error>
    Error &Unexpected<Error>::error()
    {
        return m_error;
    }

    template<class Result, class Error>
    Expected<Result, Error>::Expected(const Result &result)
        : m_data(result)
    {
    }

    template<class Result, class Error>
    Expected<Result, Error>::Expected(Result &&result)
        : m_data(std::move(result))
    {
    }

    template<class Result, class Error>
    Expected<Result, Error>::Expected(const Unexpected<Error> &error)
        : m_data(error.error())
    {
    }

    template<class Result, class Error>
    Expected<Result, Error>::Expected(Unexpected<Error> &&error)
        : m_data(std::move(error.error()))
    {
    }

    template<class Result, class Error>
    bool Expected<Result, Error>::has_value() const
    {
        return std::holds_alternative<Result>(m_data);
    }

    template<class Result, class Error>
    bool Expected<Result, Error>::has_error() const
    {
        return std::holds_alternative<Error>(m_data);
    }

    template<class Result, class Error>
    Expected<Result, Error>::operator bool() const
    {
        return std::holds_alternative<Result>(m_data);
    }

    template<class Result, class Error>
    const Result &Expected<Result, Error>::value() const
    {
        return std::get<Result>(m_data);
    }

    template<class Result, class Error>
    Result &Expected<Result, Error>::value()
    {
        return std::get<Result>(m_data);
    }

    template<class Result, class Error>
    const Error &Expected<Result, Error>::error() const
    {
        return std::get<Error>(m_data);
    }

    template<class Result, class Error>
    Error &Expected<Result, Error>::error()
    {
        return std::get<Error>(m_data);
    }
}
