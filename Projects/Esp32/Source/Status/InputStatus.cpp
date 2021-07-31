#include "InputStatus.h"

namespace Status
{


InputStatus::InputStatus(InputType inputType)
{
    _inputData.Type = inputType;
}

InputStatus::InputStatus()
{
}

InputStatus::~InputStatus()
{
}

void InputStatus::SetType(InputType inputType)
{
    _inputData.Type = inputType;
}

bool InputStatus::DoGetDigitalLevel()
{
    return _inputData.Data.Digital.State;
}

bool InputStatus::DoGetDigitalLevelDebounce()
{
    return _inputData.Data.Digital.MaskedState;
}

uint16_t InputStatus::DoGetAnalogLevel()
{
    return _inputData.Data.Analog.LiveLevel;
}

void InputStatus::DoSetDigitalLevel(bool value)
{
    // Set the State of the input
    _inputData.Data.Digital.State = value;

    // If the intput is different than the last masked value
    if (_inputData.Data.Digital.MaskedState != value)
    {
        // Increase the hit count
        _inputData.Data.Digital.HitCount++;
        // If the has changed and kept for MaxHitCount, update the masked value
        if (_inputData.Data.Digital.HitCount > MaxHitCount)
            {
               _inputData.Data.Digital.MaskedState = value;
               _inputData.Data.Digital.HitCount = 0;
            }
    }
    else
        _inputData.Data.Digital.HitCount = 0;
}

void InputStatus::DoSetAnalogLevel(uint16_t value)
{
    _inputData.Data.Analog.LiveLevel = value;
}

InputType InputStatus::GetType()
{
    return _inputData.Type;
}

} // namespace Configuration