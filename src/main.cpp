#include "sup_class/pch.h"

#include "sdl_app.h"

int main() {
    SDLApp app;

    while (app.isRunning()) app.run();
    
    app.stop();

    return 0;
}
