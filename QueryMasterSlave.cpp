// QueryMasterSlave.cpp
//
// Example: Distinguish which Orbbec devices are set as PRIMARY (master)
// versus SECONDARY (slave) in a multi‐device sync configuration.

#include <iostream>
#include <vector>
#include <memory>

#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"

// Helper to convert sync mode enum to a string
static const char* syncModeToString(const ob::MultiDeviceSyncConfig& cfg) {
    switch (cfg.syncMode) {
        case OB_MULTI_DEVICE_SYNC_MODE_DISABLED:  return "DISABLED";
        case OB_MULTI_DEVICE_SYNC_MODE_PRIMARY:   return "PRIMARY (MASTER)";
        case OB_MULTI_DEVICE_SYNC_MODE_SECONDARY: return "SECONDARY (SLAVE)";
        default:                                  return "UNKNOWN";
    }
}

int main() {
    try {
        // 1) Create context and query device list
        ob::Context context;
        auto devList = context.queryDeviceList();
        uint32_t devCount = devList->deviceCount();
        std::cout << "Found " << devCount << " Orbbec device"
                  << (devCount == 1 ? "" : "s") << ".\n\n";

        if (devCount == 0) {
            std::cerr << "No cameras detected. Exiting.\n";
            return 0;
        }

        // 2) Gather all shared_ptr<ob::Device> into a vector
        std::vector<std::shared_ptr<ob::Device>> allDevices;
        allDevices.reserve(devCount);
        for (uint32_t i = 0; i < devCount; ++i) {
            auto dev = devList->getDevice(i);
            if (dev) {
                allDevices.push_back(dev);
            }
        }

        // 3) Partition into primary and secondary lists
        std::vector<std::shared_ptr<ob::Device>> primaryDevices;
        std::vector<std::shared_ptr<ob::Device>> secondaryDevices;
        primaryDevices.reserve(devCount);
        secondaryDevices.reserve(devCount);

        for (auto& dev : allDevices) {
            // getMultiDeviceSyncConfig() returns a struct containing:
            //   - syncMode (PRIMARY, SECONDARY, or DISABLED)
            //   - optional masterDeviceIndex (if the SDK supports chaining)
            ob::MultiDeviceSyncConfig cfg = dev->getMultiDeviceSyncConfig();

            if (cfg.syncMode == OB_MULTI_DEVICE_SYNC_MODE_PRIMARY) {
                primaryDevices.push_back(dev);
            }
            else if (cfg.syncMode == OB_MULTI_DEVICE_SYNC_MODE_SECONDARY) {
                secondaryDevices.push_back(dev);
            }
            // If cfg.syncMode == DISABLED, you could choose to ignore or list separately
        }

        // 4) Print out each device and its role
        for (uint32_t i = 0; i < allDevices.size(); ++i) {
            auto dev = allDevices[i];
            auto info = dev->getDeviceInfo();
            ob::MultiDeviceSyncConfig cfg = dev->getMultiDeviceSyncConfig();

            std::cout << "------ Device [" << i << "] ------\n";
            std::cout << " Name           : " << info->name() << "\n";
            std::cout << " Serial Number  : " << info->serialNumber() << "\n";
            std::cout << " Sync Mode      : " << syncModeToString(cfg) << "\n\n";
        }

        // 5) Summary counts
        std::cout << "\nSummary:\n";
        std::cout << "  PRIMARY devices (" << primaryDevices.size() << "):\n";
        for (auto& dev : primaryDevices) {
            std::cout << "    • " << dev->getDeviceInfo()->serialNumber() << "\n";
        }
        std::cout << "  SECONDARY devices (" << secondaryDevices.size() << "):\n";
        for (auto& dev : secondaryDevices) {
            std::cout << "    • " << dev->getDeviceInfo()->serialNumber() << "\n";
        }

        return 0;
    }
    catch (const ob::Error& e) {
        std::cerr << "[Orbbec SDK error]\n"
                  << "  Function: " << e.getName()   << "\n"
                  << "  Args    : " << e.getArgs()   << "\n"
                  << "  Message : " << e.getMessage()<< "\n"
                  << "  Type    : " << e.getExceptionType() << "\n";
        return 1;
    }
}
