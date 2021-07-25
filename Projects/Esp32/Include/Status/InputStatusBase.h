#pragma once

#include "HalCommon.h"
#include "DebugAssert.h"
#include "string.h"

namespace Status
{

class InputStatusBase
{
public:
    InputStatusBase()
    {

    }
    ~InputStatusBase()
    {

    }

    /// @brief Get Digital Value
    virtual bool GetDigitalLevel()
    {
        DebugAssertMessage(GetType() == Hal::InputType::Digital, "Input is not configured as Digital.");
        return DoGetDigitalLevel();
    }

    /// @brief Get Analog Value
    virtual bool GetAnalogLevel()
    {
       DebugAssertMessage(GetType() == Hal::InputType::Analog, "Input is not configured as Analog.");
        return false;
    }
    
    /// @brief Input Type
    virtual Hal::InputType GetType() = 0;
private:
    /// @brief Get Analog Value
    virtual uint16_t DoGetAnalogLevel() = 0;
    /// @brief Get Digital Value
    virtual bool DoGetDigitalLevel() = 0;
private:
    /// @brief	Hide Copy constructor.
    InputStatusBase(const InputStatusBase &) = delete;

    /// @brief	Hide Assignment operator.
    InputStatusBase &operator=(const InputStatusBase &) = delete;

    /// @brief	Hide Move constructor.
    InputStatusBase(InputStatusBase &&) = delete;

    /// @brief	Hide Move assignment operator.
    InputStatusBase &operator=(InputStatusBase &&) = delete;
};

} // namespace Configuration