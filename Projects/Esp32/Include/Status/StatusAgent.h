#pragma once

#include "Hardware.h"
#include "BoardStatus.h"

namespace Status
{

class StatusAgent
{
public:
    StatusAgent();

    void Initialize();

    static inline StatusAgent *Instance()
    {
        if (_statusAgent == nullptr)
        {
            _statusAgent = new StatusAgent();
        }
        return _statusAgent;
    }

private:
    static StatusAgent *_statusAgent;
    BoardStatus * _boardStatus = nullptr;
    
private:
    /// @brief	Hide Copy constructor.
    StatusAgent(const StatusAgent &) = delete;

    /// @brief	Hide Assignment operator.
    StatusAgent &operator=(const StatusAgent &) = delete;

    /// @brief	Hide Move constructor.
    StatusAgent(StatusAgent &&) = delete;

    /// @brief	Hide Move assignment operator.
    StatusAgent &operator=(StatusAgent &&) = delete;
};

} // namespace Applications