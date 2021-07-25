#pragma once

#include "HalCommon.h"
#include "DebugAssert.h"
#include "string.h"

namespace Status
{

class InputStatusBase
{
public:
    InputStatusBase(const char* name)
    {
        uint32_t len = strlen(name);
        DebugAssertMessage(len < _name.size(), "Name Size too long, size:%d, supported:%d", len, _name.size());
        memset(_name.data(), 0, _name.size());
        memcpy(_name.data(), name, len);
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

    const char* GetName() const
    {
        return _name.data();
    }
    
    /// @brief Input Type
    virtual Hal::InputType GetType() = 0;
private:
    /// @brief Get Analog Value
    virtual uint16_t DoGetAnalogLevel() = 0;
    /// @brief Get Digital Value
    virtual bool DoGetDigitalLevel() = 0;
    static const uint8_t NameSize = 12;
    std::array<char, NameSize> _name;
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