
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <cstring>

int main() {
    try {
        // 1) Create a Context and query the list of connected Orbbec devices.
        ob::Context ctx;
        auto deviceList = ctx.queryDeviceList();
        uint32_t count = deviceList->deviceCount();
        std::cout << "\nFound " << count << " Orbbec device"
                  << (count == 1 ? "" : "s") << ".\n\n";

        if (count == 0) {
            std::cout << "No cameras detected. Exiting.\n";
            return 0;
        }

        // 2) Iterate through each device, collect info, and print it nicely.
        for (uint32_t i = 0; i < count; ++i) {
            auto dev  = deviceList->getDevice(i);
            auto info = dev->getDeviceInfo();
            auto cfg = dev->getMultiDeviceSyncConfig();
            

            // Header
            std::cout << "============================================\n";
            std::cout << " Device [" << i << "]\n";
            std::cout << "============================================\n";

            // Basic Info (aligned into two columns)
            struct Field { const char* label; std::string value; };
            std::vector<Field> fields = {
                { "Name",             info->name() },
                { "Serial Number",    info->serialNumber() },
                { "Firmware Version", info->firmwareVersion() },
                { "Connection Type",  info->connectionType() }
            };

            // Determine the maximum label width for column alignment
            size_t maxLabelWidth = 0;
            for (auto& fld : fields) {
                maxLabelWidth = std::max(maxLabelWidth, std::strlen(fld.label));
            }

            // Print each field as: "  <Label>: <Value>"
            for (auto& fld : fields) {
                std::cout << "  "
                          << std::left << std::setw(int(maxLabelWidth)) << fld.label
                          << " : "
                          << fld.value << "\n";
            }

            // List Sensors
            if(cfg.syncMode == OB_MULTI_DEVICE_SYNC_MODE_PRIMARY) {
                std::cout << "Device [" << i << "] is the master device.\n";
            } else {
                std::cout << "Device [" << i << "] is a slave device.\n";
            }
            auto sensorList = dev->getSensorList();
            uint32_t sensorCount = sensorList->count();
            std::cout << "\n  Sensors (" << sensorCount << "):\n";

            // Print each sensor on its own indented line
            for (uint32_t s = 0; s < sensorCount; ++s) {
                auto sensor = sensorList->getSensor(s);
                std::string sensorName;
                switch (sensor->type()) {
                    case OB_SENSOR_COLOR:      sensorName = "Color";      break;
                    case OB_SENSOR_DEPTH:      sensorName = "Depth";      break;
                    case OB_SENSOR_IR:         sensorName = "IR";         break;
                    case OB_SENSOR_IR_LEFT:    sensorName = "IR Left";    break;
                    case OB_SENSOR_IR_RIGHT:   sensorName = "IR Right";   break;
                    case OB_SENSOR_GYRO:       sensorName = "Gyro";       break;
                    case OB_SENSOR_ACCEL:      sensorName = "Accel";      break;
                    default:                   sensorName = "Unknown";    break;
                }
                std::cout << "    - " << sensorName << "\n";
            }

            std::cout << "\n";  // extra blank line between devices
        }
    }
    catch (const ob::Error& e) {
        std::cerr << "[Orbbec SDK error]\n"
                  << "  Function : " << e.getName() << "\n"
                  << "  Arguments: " << e.getArgs() << "\n"
                  << "  Message  : " << e.getMessage() << "\n"
                  << "  Type     : " << e.getExceptionType() << "\n";
        return 1;
    }

    return 0;
}
