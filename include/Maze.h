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

    // Cizim (Adim 2.2'de doldurulacak)
    void draw(sf::RenderWindow& window) const;

    // Sorgular (Adim 3'te hareket icin kullanilacak)
    Tile  at(int col, int row) const;
    bool  isWall(int col, int row) const;
    bool  isInBounds(int col, int row) const;

    int cols() const { return Constants::MAZE_COLS; }
    int rows() const { return Constants::MAZE_ROWS; }

private:
    // 28x31 sabit harita verisi
    static const std::vector<std::string> RAW_MAP;

    // Karakter haritasini Tile dizisine cevirip burada tutuyoruz
    std::vector<std::vector<Tile>> grid_;

    // Karakter -> Tile donusumu
    static Tile charToTile(char c);
};