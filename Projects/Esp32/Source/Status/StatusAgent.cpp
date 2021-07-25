#include "StatusAgent.h"

namespace Status
{

StatusAgent *StatusAgent::_statusAgent;

StatusAgent::StatusAgent()
{
    if (_statusAgent == nullptr)
    {
        _statusAgent = this;
    }
    else
    {
        printf("Error!\n");
    }
}

void StatusAgent::Initialize()
{
    _boardStatus = new BoardStatus();
    _inputStatusList = new InputStatusList();
}

} // namespace Applications
