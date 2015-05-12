#include "vrfrontend.h"
#include "sampleapp.h"

int main( int argc, char **argv) {
    // The first argument to the program tells us which of the known VR setups to start.
    std::string setupStr;
    VRFrontend *app;

    // This opens up the graphics window, and starts connections to input devices, but doesn't actually start rendering yet.
    app = new VRFrontend(setupStr, new SampleApp());

    // This starts the rendering/input processing loop
    app->run();

    delete app;

    return 0;
}
