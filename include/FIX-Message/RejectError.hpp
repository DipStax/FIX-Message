#pragma once

#include <string>

#include "FIX-Message/meta/using.hpp"

namespace fix
{
    struct RejectError
    {
        static constexpr const uint8_t InvalidTag = 0;      ///< Reason of reject when a tag is invalid.
        static constexpr const uint8_t ReqTagMissing = 1;   ///< Reason of reject when a required tag is missing.
        static constexpr const uint8_t NotSupporType = 2;   ///< Reason of reject when a tag is not supported by the message.
        static constexpr const uint8_t UndefineTag = 3;     ///< Reason of reject when a tag is unknow.
        static constexpr const uint8_t EmptyValue = 4;      ///< Reason of reject when a tag's value is empty.
        static constexpr const uint8_t ValueOORange = 5;    ///< Reason of reject when a tag's value is out of range.
        static constexpr const uint8_t IncorrectFormat = 6; ///< Reason of reject when a tag's value is incorrectly formated.
        static constexpr const uint8_t DecryptionIssue = 7; ///< Reason of reject when their a decryption issue

        uint8_t Reason;
        std::string Message;
        fix::TagName Tag{};
    };
}