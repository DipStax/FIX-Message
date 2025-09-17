#pragma once

#include <string>

#include "FIX-Message/meta/using.hpp"

namespace fix
{
    struct RejectError
    {
        static constexpr const uint8_t InvalidTag = 1;              ///< Reason of reject when a tag is invalid.
        static constexpr const uint8_t ReqTagMissing = 2;           ///< Reason of reject when a required tag is missing.
        static constexpr const uint8_t TagNotDefine = 3;            ///< Reason of reject when a tag is unknow.
        static constexpr const uint8_t UndefineTag = 4;             ///< Reason of reject when a tag is unknow.
        static constexpr const uint8_t EmptyValue = 5;              ///< Reason of reject when a tag's value is empty.
        static constexpr const uint8_t ValueOORange = 6;            ///< Reason of reject when a tag's value is out of range.
        static constexpr const uint8_t IncorrectFormat = 7;         ///< Reason of reject when a tag's value is incorrectly formated.
        static constexpr const uint8_t SignatureProblem = 8;        ///< Reason of reject when a problem with the signature occured
        static constexpr const uint8_t CompIdProblem = 9;           ///< Reason of reject when a partie ID is invalid
        static constexpr const uint8_t SendingTimeAccuracy = 10;    ///< Reason of reject when the sending time is inacurate
        static constexpr const uint8_t InvalidMsgType = 10;         ///< Reason of reject when the message type is invalid

        uint8_t Reason;
        std::string Message;
        fix::TagName Tag{};
    };
}