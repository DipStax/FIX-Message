#pragma once

#include <string>

namespace fix
{
    struct RejectError
    {
        static constexpr const char InvalidTag[] = "0";      ///< Reason of reject when a tag is invalid.
        static constexpr const char ReqTagMissing[] = "1";   ///< Reason of reject when a required tag is missing.
        static constexpr const char NotSupporType[] = "2";   ///< Reason of reject when a tag is not supported by the message.
        static constexpr const char UndefineTag[] = "3";     ///< Reason of reject when a tag is unknow.
        static constexpr const char EmptyValue[] = "4";      ///< Reason of reject when a tag's value is empty.
        static constexpr const char ValueOORange[] = "5";    ///< Reason of reject when a tag's value is out of range.
        static constexpr const char IncorrectFormat[] = "6"; ///< Reason of reject when a tag's value is incorrectly formated.
        static constexpr const char DecryptionIssue[] = "7"; ///< Reason of reject when their a decryption issue

        std::string Reason;
        std::string Message;
        std::string Tag;
    };
}