#include "ApplicationAgent.h"

namespace Applications
{

ApplicationAgent *ApplicationAgent::_applications;

ApplicationAgent::ApplicationAgent()
{
    if (_applications == nullptr)
    {
        _applications = this;
    }
    else
    {
        printf("Error!\n");
    }
}

void ApplicationAgent::Initialize()
{
    // _wifiService = new WifiService();
    // _firmwareUpdateService = new FirmwareUpdateService();
    _laserControlService = new LaserControlService();
}

} // namespace Applications
