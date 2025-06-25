#include <iostream>
#include <memory>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/h/Property.h"

int main() {
    ob::Context ctx;
    auto list = ctx.queryDeviceList();
    if (!list || list->deviceCount() == 0) {
        std::cerr << "No Orbbec device found\n";
        return 1;
    }
    auto dev   = list->getDevice(0);
    auto count = dev->getSupportedPropertyCount();
    std::cout << "Available properties (" << count << "):\n";
    for (uint32_t i = 0; i < count; ++i) {
        auto p = dev->getSupportedProperty(i);
        std::cout
            << i << ": " << p.name
            << " (id=" << p.id
            << ", type=" << p.type << ")\n";
    }
    return 0;
}
