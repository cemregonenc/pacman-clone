#include "Ghost.h"
#include <cmath>


Ghost::Ghost(Personality p, sf::Color color, sf::Vector2f startPos)
    : personality_(p)
    , color_(color)
    , position_(startPos)
    , startPos_(startPos)
    , direction_(Direction::Left)
    , speed_(Constants::TILE_SIZE * 4.f)        // Pac-Man'den biraz yavas
    , radius_(Constants::TILE_SIZE * 0.45f)
    , mode_(Mode::Chase)
    , animTime_(0.f)
{
}


// Adim 5.1'de henuz hareket yok, sadece zaman ilerletme.
// 5.2 ve 5.3'te chooseDirection() icini dolduracagiz.
void Ghost::update(sf::Time deltaTime, const Maze& /*maze*/,
                   sf::Vector2f /*playerPos*/, Direction /*playerDir*/) {
    animTime_ += deltaTime.asSeconds();
}


// Klasik hayalet sekli:
//   - Govde: yarim daire (ust) + dikdortgen (alt)
//   - Alt kenarda 3 dalga (etek)
//   - 2 buyuk goz: beyaz daire + siyah goz bebek (yone bakar)
void Ghost::draw(sf::RenderWindow& window) const {
    const float r = radius_;

    // Frightened modda mavimsi, normalde kendi rengi
    sf::Color bodyColor = (mode_ == Mode::Frightened)
                          ? Constants::Colors::GHOST_FRIGHTENED
                          : color_;

    // ----- GOVDE (yarim daire + dikdortgen) -----
    // Yarim daire (ust kafa)
    sf::CircleShape head(r, 24);
    head.setFillColor(bodyColor);
    head.setOrigin(r, r);
    head.setPosition(position_);
    window.draw(head);

    // Dikdortgen (alt govde)
    sf::RectangleShape body(sf::Vector2f(r * 2.f, r));
    body.setFillColor(bodyColor);
    body.setOrigin(r, 0.f);
    body.setPosition(position_.x, position_.y);
    window.draw(body);

    // ----- ALTTAKI 3 DALGA (etek) -----
    // Eteklerin yatay konumu animasyonla sallanir
    float wave = std::sin(animTime_ * 8.f) * 2.f;
    for (int i = 0; i < 3; ++i) {
        sf::CircleShape bump(r / 3.f, 12);
        bump.setFillColor(bodyColor);
        bump.setOrigin(r / 3.f, r / 3.f);
        bump.setPosition(
            position_.x - r + r / 3.f + i * (r * 2.f / 3.f) + wave,
            position_.y + r
        );
        window.draw(bump);
    }

    // ----- GOZLER -----
    // Goz bebek yone gore kayar
    sf::Vector2f pupilOffset = directionVector(direction_) * (r * 0.15f);

    // Sol goz
    sf::CircleShape eyeLeft(r * 0.25f, 16);
    eyeLeft.setFillColor(sf::Color::White);
    eyeLeft.setOrigin(r * 0.25f, r * 0.25f);
    eyeLeft.setPosition(position_.x - r * 0.35f, position_.y - r * 0.15f);
    window.draw(eyeLeft);

    sf::CircleShape pupilLeft(r * 0.12f, 12);
    pupilLeft.setFillColor(sf::Color::Black);
    pupilLeft.setOrigin(r * 0.12f, r * 0.12f);
    pupilLeft.setPosition(
        position_.x - r * 0.35f + pupilOffset.x,
        position_.y - r * 0.15f + pupilOffset.y
    );
    window.draw(pupilLeft);

    // Sag goz
    sf::CircleShape eyeRight(r * 0.25f, 16);
    eyeRight.setFillColor(sf::Color::White);
    eyeRight.setOrigin(r * 0.25f, r * 0.25f);
    eyeRight.setPosition(position_.x + r * 0.35f, position_.y - r * 0.15f);
    window.draw(eyeRight);

    sf::CircleShape pupilRight(r * 0.12f, 12);
    pupilRight.setFillColor(sf::Color::Black);
    pupilRight.setOrigin(r * 0.12f, r * 0.12f);
    pupilRight.setPosition(
        position_.x + r * 0.35f + pupilOffset.x,
        position_.y - r * 0.15f + pupilOffset.y
    );
    window.draw(pupilRight);
}


// Yardimcilar
sf::Vector2i Ghost::currentTile() const {
    int col = static_cast<int>(position_.x / Constants::TILE_SIZE);
    int row = static_cast<int>(position_.y / Constants::TILE_SIZE);
    return { col, row };
}


bool Ghost::atTileCenter() const {
    float cx = currentTile().x * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
    float cy = currentTile().y * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
    const float tolerance = 2.f;
    return std::abs(position_.x - cx) < tolerance &&
           std::abs(position_.y - cy) < tolerance;
}


sf::Vector2f Ghost::directionVector(Direction d) const {
    switch (d) {
        case Direction::Up:    return { 0.f, -1.f };
        case Direction::Down:  return { 0.f,  1.f };
        case Direction::Left:  return {-1.f,  0.f };
        case Direction::Right: return { 1.f,  0.f };
        default:               return { 0.f,  0.f };
    }
}


bool Ghost::canMoveInDirection(Direction d, const Maze& maze) const {
    sf::Vector2i tile = currentTile();
    sf::Vector2f v = directionVector(d);
    int nextCol = tile.x + static_cast<int>(v.x);
    int nextRow = tile.y + static_cast<int>(v.y);
    return !maze.isWall(nextCol, nextRow);
}


// 5.2 ve 5.3'te doldurulacak
void Ghost::chooseDirection(const Maze& /*maze*/, sf::Vector2f /*playerPos*/,
                            Direction /*playerDir*/) {
    // bos
}