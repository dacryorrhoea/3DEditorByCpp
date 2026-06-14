#include "main_classes/sdl_app.h"
#include <iostream>
#include <exception>

int main() {
    try {
        SDLApp app;

        while (app.isRunning()) {
            app.run();
        }

        app.stop();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal exception in main: "
                  << e.what()
                  << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal exception in main"
                  << std::endl;
        return 1;
    }
}