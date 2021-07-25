#include "InputStatus.h"

namespace Status
{


InputStatus::InputStatus(const char* name, Hal::InputType inputType) : InputStatusBase(name)
{
    _inputData.Type = inputType;
}

InputStatus::~InputStatus()
{
}

bool InputStatus::DoGetDigitalLevel()
{
    return _inputData.Data.Digital.State;
}

uint16_t InputStatus::DoGetAnalogLevel()
{
    return _inputData.Data.Analog.LiveLevel;
}

InputType InputStatus::GetType()
{
    return _inputData.Type;
}

} // namespace Configuration