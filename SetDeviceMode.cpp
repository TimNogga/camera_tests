#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <cstring>


int main() {
    ob::Context ctx;
    auto devList = ctx.queryDeviceList();

    // prepare a config for PRIMARY mode
    OBMultiDeviceSyncConfig cfg{};
    cfg.syncMode = OB_MULTI_DEVICE_SYNC_MODE_PRIMARY;

    // apply to every connected camera
    for (uint32_t i = 0; i < devList->deviceCount(); ++i) {
        auto dev = devList->getDevice(i);
        dev->setMultiDeviceSyncConfig(cfg);
    }

    return 0;
}