#pragma once
#include <SFML/Graphics.hpp>

// Projenin her yerinde kullanilacak sabitler ve renk paleti
namespace Constants {

    // ---------- Tile / Labirent ----------
    constexpr int TILE_SIZE = 24;          // Her hucre kac piksel
    constexpr int MAZE_COLS = 28;          // Klasik Pac-Man genisligi
    constexpr int MAZE_ROWS = 31;          // ve yuksekligi

    // ---------- Pencere ----------
    constexpr int UI_HEIGHT = 80;          // Skor paneli icin alt bosluk
    constexpr int WINDOW_WIDTH  = MAZE_COLS * TILE_SIZE;
    constexpr int WINDOW_HEIGHT = MAZE_ROWS * TILE_SIZE + UI_HEIGHT;

    // ---------- Zamanlama ----------
    constexpr int FPS = 60;

    // ---------- Renkler ----------
    namespace Colors {
        const sf::Color BACKGROUND       (255, 192, 203);      // Koyu lacivert
        const sf::Color WALL             (75, 0, 130);     // Mavi duvar
        const sf::Color WALL_GLOW        (147, 112, 219);     // Duvar parlama
        const sf::Color PELLET           (255, 184, 151);   // Bej noktalar
        const sf::Color POWER_PELLET     (255, 222, 89);    // Sari guc meyvesi
        const sf::Color PACMAN           (255, 235, 59);    // Pac-Man sarisi
        const sf::Color GHOST_RED        (255, 0, 0);       // Blinky
        const sf::Color GHOST_PINK       (255, 184, 222);   // Pinky
        const sf::Color GHOST_CYAN       (0, 255, 255);     // Inky
        const sf::Color GHOST_ORANGE     (255, 165, 0);     // Clyde
        const sf::Color GHOST_FRIGHTENED (33, 33, 222);     // Korkmus mod
        const sf::Color UI_TEXT          (255, 255, 255);
        const sf::Color UI_HIGHLIGHT     (255, 235, 59);
    }

    // Oyunun bulunabilecegi durumlar
    enum class GameState {
        Menu,
        Playing,
        Paused,
        LevelComplete,
        GameOver
    };

}