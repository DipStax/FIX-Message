#include "Exception.hpp"

namespace fix
{

    RejectException::RejectException(const std::string &_reason, const std::string &_msg) noexcept
        : m_reason(_reason), m_msg(_msg)
    {
    }

    const char *RejectException::what() const noexcept
    {
        return m_msg.c_str();
    }

    const char *RejectException::reason() const noexcept
    {
        return m_reason.c_str();
    }

}