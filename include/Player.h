#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Maze.h"

// Oyuncu (Pac-Man) sinifi.
// Klavye ile yon degistirir, surekli son yonde ilerler.
// Adim 3.3'te duvar carpismasi, 3.4'te ozel fotograf destegi eklenecek.
class Player {
public:
    enum class Direction { None, Up, Down, Left, Right };

    Player();

    void handleInput(const Maze& maze);
    void update(sf::Time deltaTime, const Maze& maze);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f position() const { return position_; }
    Direction    direction() const { return direction_; }
    void         reset();

private:
    void drawClassic(sf::RenderWindow& window) const;
    void drawCustomImage(sf::RenderWindow& window) const;
 

    // Yardimcilar
    sf::Vector2i currentTile() const;
    bool         atTileCenter() const;
    sf::Vector2f directionVector(Direction d) const;
    bool         canMoveInDirection(Direction d, const Maze& maze) const;


    // Uyeler
    sf::Vector2f position_;
    Direction    direction_;
    Direction    desiredDirection_;
    float        animTime_;
    float        radius_;
    float        speed_;
    sf::Texture  customTexture_;
    sf::Sprite   customSprite_;
    bool         useCustomImage_;
};