#pragma once

#include <string>

namespace fix
{
    class RejectException : public std::exception
    {
        public:
            RejectException(const std::string &_reason, const std::string &_msg) noexcept;

            const char *what() const noexcept;
            const char *reason() const noexcept;

            static constexpr const char InvalidTag[] = "0";      ///< Reason of reject when a tag is invalid.
            static constexpr const char ReqTagMissing[] = "1";   ///< Reason of reject when a required tag is missing.
            static constexpr const char NotSupporType[] = "2";   ///< Reason of reject when a tag is not supported by the message.
            static constexpr const char UndefineTag[] = "3";     ///< Reason of reject when a tag is unknow.
            static constexpr const char EmptyValue[] = "4";      ///< Reason of reject when a tag's value is empty.
            static constexpr const char ValueOORange[] = "5";    ///< Reason of reject when a tag's value is out of range.
            static constexpr const char IncorrectFormat[] = "6"; ///< Reason of reject when a tag's value is incorrectly formated.
            static constexpr const char DecryptionIssue[] = "7"; ///< Reason of reject when their a decryption issue

        private:
            const std::string m_reason;
            const std::string m_msg;
    };
}