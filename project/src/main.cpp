#include "vrfrontend.h"
#include "sampleapp.h"

int main( int argc, char **argv) {
    // The first argument to the program tells us which of the known VR setups to start.
    std::string setupStr;
    VrFrontend *app;

if (argc >= 2)
  {  setupStr = std::string(argv[1]);
  }


    // This opens up the graphics window, and starts connections to input devices, but doesn't actually start rendering yet.
    app = new VrFrontend(setupStr, new SampleApp());

    // This starts the rendering/input processing loop
    app->run();

    delete app;

    return 0;
}
