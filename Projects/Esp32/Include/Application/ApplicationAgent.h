#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "FirmwareUpdateService.h"
#include "LaserControlService.h"
#include "MenuService.h"
#include "InputScannerService.h"

namespace Applications
{

class ApplicationAgent
{
public:
    ApplicationAgent();

    void Initialize();

    static inline ApplicationAgent *Instance()
    {
        if (_applications == nullptr)
        {
            _applications = new ApplicationAgent();
        }
        return _applications;
    }

    LaserControlService* GetLaserControlService() { return _laserControlService; }
    MenuService* GetMenuService() { return _menuService; }
    InputScannerService* GetInputScannerService() { return _inputScanner; }

private:
    static ApplicationAgent *_applications;
    LaserControlService * _laserControlService;
    MenuService * _menuService;
    InputScannerService * _inputScanner;

private:
    /// @brief	Hide Copy constructor.
    ApplicationAgent(const ApplicationAgent &) = delete;

    /// @brief	Hide Assignment operator.
    ApplicationAgent &operator=(const ApplicationAgent &) = delete;

    /// @brief	Hide Move constructor.
    ApplicationAgent(ApplicationAgent &&) = delete;

    /// @brief	Hide Move assignment operator.
    ApplicationAgent &operator=(ApplicationAgent &&) = delete;
};

} // namespace Applications