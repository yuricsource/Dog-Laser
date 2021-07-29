#pragma once

#include <stdio.h>
#include <cstdint>
#include <cstring>
#include <cctype>
#include <array>

namespace Configuration
{
    // Device Name
    using DeviceName = std::array<char, 21>;

    // Static Input Configuration Index
    enum class InputIndex : uint8_t
    {
        ButtonOk,
        ButtonBack,
        PotLaserPower,
        PotLaserDelay
    };
} // namespace Common