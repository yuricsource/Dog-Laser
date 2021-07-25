#pragma once

#include "HalCommon.h"
#include "InputStatusBase.h"

namespace Status
{
using Hal::InputType;

class InputStatus : public InputStatusBase
{
public:
    InputStatus(const char* name, Hal::InputType inputType);

    ~InputStatus();

private:
    /// @brief Do Get Digital Level
    bool DoGetDigitalLevel() override;
    
    /// @brief Do Get Analog Level
    uint16_t DoGetAnalogLevel() override;
    
    /// @brief Get Type
    InputType GetType() override;

    struct InputData 
    {
        InputType Type;
        union
        {
            struct
            {
                bool State;
                bool MaskedState;
            } Digital;
            struct
            {
                uint16_t LiveLevel;
                uint16_t MaskedLevel;
            } Analog;
        } Data;
    };
    static_assert(sizeof(InputData) == 6, "InputData has wrong size, compiler error.");

    InputData _inputData = {};
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