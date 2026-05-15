#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.h"

// Labirenti yoneten sinif.
// Harita 28 sutun x 31 satir tile'dan olusur. Her tile bir karakter:
//   '#' duvar
//   '.' yenebilir nokta (pellet)
//   'o' guc meyvesi (power pellet)
//   ' ' bos (gecis alani veya hayaletlerin baslangic odasi)
//   '-' hayalet kapisi (Pac-Man gecemez, hayaletler gecer)
class Maze {
public:
    enum class Tile {
        Empty,
        Wall,
        Pellet,
        PowerPellet,
        GhostGate
    };

    Maze();

    // Cizim
    void draw(sf::RenderWindow& window) const;

    // Sorgular
    Tile  at(int col, int row) const;
    bool  isWall(int col, int row) const;
    bool  isInBounds(int col, int row) const;

    // Pac-Man bir tile'in uzerinde durdugunda cagrilir.
    // Tile pellet veya power pellet ise yer ve skoru dondurur.
    // Bos veya duvar ise 0 dondurur (yenecek bir sey yok).
    int   eatPelletAt(int col, int row);

    // Kalan pellet sayisi (level complete kontrolu icin, Adim 4.4'te kullanilacak)
    int   remainingPellets() const { return pelletCount_; }

    int cols() const { return Constants::MAZE_COLS; }
    int rows() const { return Constants::MAZE_ROWS; }

private:
    static const std::vector<std::string> RAW_MAP;
    std::vector<std::vector<Tile>> grid_;
    static Tile charToTile(char c);
    int pelletCount_;
};