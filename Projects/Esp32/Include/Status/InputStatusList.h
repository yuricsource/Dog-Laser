#pragma once

#include "HalCommon.h"
#include "InputStatus.h"

namespace Status
{

class InputStatusList
{
public:
    InputStatusList();
    ~InputStatusList();

private:

private:
    /// @brief	Hide Copy constructor.
    InputStatusList(const InputStatusList &) = delete;

    /// @brief	Hide Assignment operator.
    InputStatusList &operator=(const InputStatusList &) = delete;

    /// @brief	Hide Move constructor.
    InputStatusList(InputStatusList &&) = delete;

    /// @brief	Hide Move assignment operator.
    InputStatusList &operator=(InputStatusList &&) = delete;
};

} // namespace Configuration