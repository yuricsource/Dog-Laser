#pragma once

#include "HalCommon.h"
#include "InputStatusBase.h"

namespace Status
{
using Hal::InputType;

class InputStatus : public InputStatusBase
{
public:
    InputStatus(InputType inputType);
    InputStatus();

    /// @brief Get Type
    InputType GetType() override;

    /// @brief Get Type
    void SetType(InputType inputType);

    ~InputStatus();

private:
    static const uint8_t MaxHitCount = 5;
    struct InputData 
    {
        InputType Type;
        uint8_t Reserved_0;
        union
        {
            struct
            {
                bool State          : 1;
                bool MaskedState    : 1;
                uint8_t HitCount    : 6;
                uint16_t Reserved_0 : 16;
            } Digital;
            struct
            {
                uint16_t LiveLevel  : 16;
                uint16_t MaskedLevel: 16;
            } Analog;
        } Data;
    };
    static_assert(sizeof(InputData) == 6, "InputData has wrong size, compiler error.");

    InputData _inputData = {};


    /// @brief Do Get Digital Level
    bool DoGetDigitalLevel() override;
    
    /// @brief Do Get Analog Level
    uint16_t DoGetAnalogLevel() override;

    /// @brief Get Digital Value
    void DoSetDigitalLevel(bool value) override;
    
    /// @brief Set Analog Value
    void DoSetAnalogLevel(uint16_t value) override;

    /// @brief Do Get Digital Debounce Level
    bool DoGetDigitalLevelDebounce() override;

private:
    /// @brief	Hide Copy constructor.
    InputStatus(const InputStatus &) = delete;

    /// @brief	Hide Assignment operator.
    InputStatus &operator=(const InputStatus &) = delete;

    /// @brief	Hide Move constructor.
    InputStatus(InputStatus &&) = delete;

    /// @brief	Hide Move assignment operator.
    InputStatus &operator=(InputStatus &&) = delete;
};

} // namespace Configuration