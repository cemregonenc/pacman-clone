#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Maze.h"

// Oyuncu (Pac-Man) sinifi.
// Adim 3.1'de sadece sabit bir konumda cizim yapiyor.
// Sonraki adimlarda hareket, duvar carpismasi ve ozel fotograf destegi eklenecek.
class Player {
public:
    // Yon enum'u (hareket icin sonradan kullanilacak)
    enum class Direction { None, Up, Down, Left, Right };

    Player();

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window) const;

    // Konum sorgusu (hayalet AI'inin Pac-Man'i takip etmesi icin)
    sf::Vector2f position() const { return position_; }

private:
    // Klasik Pac-Man yuvarlagini ciz (sari daire + animasyonlu agiz)
    void drawClassic(sf::RenderWindow& window) const;

    sf::Vector2f position_;     // Piksel cinsinden (tile merkezi)
    Direction    direction_;    // Yuzun baktigi yon
    float        animTime_;     // Agiz animasyonu icin gecen sure
    float        radius_;       // Pac-Man yaricapi
};