#pragma once

#include <limits>

#include "FIX-Message/meta/concept.hpp"

namespace fix::meta
{
    struct NumericLimit
    {
        template<IsIntegralNumber T>
        static consteval size_t MaxAlloc()
        {
            constexpr T maxVal = std::numeric_limits<T>::max();

            std::size_t digits = 0;
            T tmp = maxVal;
            do {
                tmp /= 10;
                ++digits;
            } while (tmp > 0);

            if constexpr (std::is_signed_v<T>)
                return digits + 1;
            else
                return digits;
        }

        template<IsFloatingPointNumber T, size_t Precision>
        static consteval std::size_t MaxAlloc()
        {
            T maxVal = std::numeric_limits<T>::max();
            std::size_t intDigits = 0;
            T tmp = maxVal;

            while (tmp >= T(1)) {
                tmp /= T(10);
                ++intDigits;
            }
            return intDigits + Precision + 2;
        }
    };
}