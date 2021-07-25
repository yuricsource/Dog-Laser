#include "InputStatusList.h"

namespace Status
{

InputStatusList::InputStatusList()
{
    for(uint16_t i = 0; i < _inputList.size(); i++)
    {
        _inputList[i].SetType(StaticInputConfig[i]);
    }
}

InputStatusList::~InputStatusList()
{
}

} // namespace Configuration