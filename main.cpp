#include <iostream>
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "FrameSaver.hpp"

int main() try
{
    constexpr int N_SETS = 50;                   // how many colour+depth pairs

    ob::Context ctx;
    if (ctx.queryDeviceList()->deviceCount() == 0) {
        std::cerr << "No Orbbec camera found\n";
        return 1;
    }

    ob::Pipeline pipe;
    pipe.start();               

    int saved = 0;
    while (saved < N_SETS) {
        if (auto fs = pipe.waitForFrames(1000)) {          // 1-s slice
            auto colour = fs->colorFrame();
            auto depth  = fs->depthFrame();
            if (colour && depth) {
                saveColor(colour, saved);
                saveDepthRaw(depth, saved); 
                saveDepthVis(depth, saved);

                ++saved;
            }
        }
    }

    pipe.stop();
    std::cout << "Done – " << saved << " sets written.\n";
}
catch (const ob::Error& e) {
    std::cerr << e.getMessage() << '\n';
    return 2;
}
