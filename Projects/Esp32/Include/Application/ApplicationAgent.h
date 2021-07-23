#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "FirmwareUpdateService.h"
#include "LaserControlService.h"

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

    // WifiService& GetWifi() {return *_wifiService;}
    //HttpServer& GetHttpServer(){return *_httpServer;}
    // FirmwareUpdateService& GetFirmwareUpdateService(){ return *_firmwareUpdateService; }
    LaserControlService& GetLaserControlService() { return *_laserControlService; }

private:
    static ApplicationAgent *_applications;
    // WifiService *_wifiService;
    //HttpServer *_httpServer;
    // FirmwareUpdateService * _firmwareUpdateService;
    LaserControlService * _laserControlService;

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