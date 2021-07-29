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

uint16_t InputStatus::DoGetAnalogLevel()
{
    return _inputData.Data.Analog.LiveLevel;
}

void InputStatus::DoSetDigitalLevel(bool value)
{
    _inputData.Data.Digital.State = value;
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