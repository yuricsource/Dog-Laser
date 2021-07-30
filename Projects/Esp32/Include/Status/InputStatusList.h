#pragma once

#include "HalCommon.h"
#include "InputStatus.h"
#include "DebugAssert.h"
#include "ConfigurationCommon.h"

namespace Status
{
using std::array;
using Hal::MaxDigitalInputs;
using Hal::MaxAnalogInputs;
using Hal::InputType;
using Configuration::InputIndex;

class InputStatusList
{
public:

    InputStatusList();
    ~InputStatusList();

    // @brief Max input configuration
    static constexpr uint16_t Count() { return InputCount; }

    // @brief	Check if Id is in range.
	static constexpr bool IsInRange(int logicalId)
	{
		return logicalId >= 1 && logicalId <= InputCount;
	}

    inline InputStatus& GetInput(InputIndex index)
    {
        uint8_t inputIndex = static_cast<uint8_t>(index);
        return _inputList[inputIndex];
    }

	/// @brief	Array indexer operator.
	/// @param	logicalId	Input Logical Identifier, 1 based.
	/// @return	The InputStatus instance at logicalId index.
	inline InputStatus& operator[](int logicalId)
	{
		if (!IsInRange(logicalId))
		{
			assert(logicalId >= 1 && logicalId <= InputCount);
            DebugAssertMessage(logicalId >= 1 && logicalId <= InputCount,
            "Input-Id#%d|Out of Range|\n", logicalId);
		}
		return _inputList[logicalId - 1];
	}

private:
    static const uint16_t InputCount = MaxAnalogInputs + MaxDigitalInputs;
    array <InputStatus, InputCount> _inputList;
    InputType StaticInputConfig [InputCount] = {InputType::Digital, InputType::Digital,
                                                             InputType::Analog,  InputType::Analog};
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