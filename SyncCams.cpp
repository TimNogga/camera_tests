#include <iostream>
#include <thread>
#include <chrono>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/h/ObTypes.h"
#include "/home/timnogga/camera_tests/include/FrameSaver.hpp"

int main() try {
    ob::Context ctx;
    auto list = ctx.queryDeviceList();
    if (list->deviceCount() < 2) {
        std::cerr << "need two cameras\n";
        return 1;
    }

    for (uint32_t i = 0; i < 2; ++i) {
        OBMultiDeviceSyncConfig cfg{};
        cfg.syncMode = (i == 0)
                     ? OB_MULTI_DEVICE_SYNC_MODE_PRIMARY
                     : OB_MULTI_DEVICE_SYNC_MODE_SECONDARY;
        if (i == 0) cfg.triggerOutEnable = true;
        list->getDevice(i)->setMultiDeviceSyncConfig(cfg);
    }

    ob::Pipeline slave (list->getDevice(1));
    ob::Pipeline master(list->getDevice(0));

    slave.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    master.start();

    constexpr int TARGET = 50;
    int saved = 0;

    while (saved < TARGET) {
        auto fsM = master.waitForFrames(3000);
        auto fsS = slave .waitForFrames(3000);
        if (!fsM || !fsS) { std::cout << "timeout @" << saved << '\n'; continue; }

        auto cM = fsM->colorFrame();
        auto cS = fsS->colorFrame();
        if (!cM || !cS) { std::cout << "no colour @" << saved << '\n'; continue; }

        std::cout << "pair " << saved
                  << " tsM=" << fsM->timeStamp()
                  << " tsS=" << fsS->timeStamp() << '\n';

        saveColor(cM, saved);
        saveColor(cS, saved + 1000);
        ++saved;
    }

    master.stop();
    slave .stop();
    std::cout << "done, pairs captured: " << saved << '\n';
    return 0;
}
catch (const ob::Error& e) {
    std::cerr << "[sdk] " << e.getMessage() << '\n';
    return 2;
}
