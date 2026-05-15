#include "Game.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "[HATA] " << e.what() << std::endl;
        return 1;
    }
    return 0;
}