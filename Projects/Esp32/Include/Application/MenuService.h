#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"
#include "BaseConnection.h"
#include "BaseRouteHandler.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "WebSocket.h"

namespace Applications
{

using Hal::TimeLimit;
using Utilities::Logger;
using Hal::Hardware;

class MenuService : public cpp_freertos::Thread
{
public:
    MenuService();
private:
    enum class MenuStateMachine: uint16_t
    {
        Idle
    };
    MenuStateMachine _stateMachine;
    Adafruit_SSD1306* _display;

    bool updateDisplay(const char * message) const;

    void writeDisplay(const char * message) const;

protected:
    void Run() override;

    

private:
    /// @brief	Hide Copy constructor.
    MenuService(const MenuService &) = delete;

    /// @brief	Hide Assignment operator.
    MenuService &operator=(const MenuService &) = delete;

    /// @brief	Hide Move constructor.
    MenuService(MenuService &&) = delete;

    /// @brief	Hide Move assignment operator.
    MenuService &operator=(MenuService &&) = delete;
};

} // namespace Applications