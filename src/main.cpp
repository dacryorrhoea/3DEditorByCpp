#include "main_classes/sdl_app.h"

int main() {
    SDLApp app;

    while (app.isRunning()) app.run();
    
    app.stop();

    return 0;
}
