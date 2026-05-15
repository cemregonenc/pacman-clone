#include "Player.h"
#include <cmath>

// Pac-Man'i klasik baslangic konumuna yerlestir:
// Labirentin altinda, ortada bir yerde (tile merkezine hizali)
Player::Player()
    : position_(14.f * Constants::TILE_SIZE,        // 14. sutun merkezi
                23.f * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f)
    , direction_(Direction::Right)                  // Baslangicta saga bakar
    , animTime_(0.f)
    , radius_(Constants::TILE_SIZE * 0.45f)         // Tile'in biraz icinde kalsin
{
}


// Adim 3.2'de hareket eklenecek. Su an sadece agiz animasyon zamani.
void Player::update(sf::Time deltaTime) {
    animTime_ += deltaTime.asSeconds();
}


void Player::draw(sf::RenderWindow& window) const {
    drawClassic(window);
}


// Klasik Pac-Man: sari daire + animasyonlu ucgen agiz.
// (Menudeki Pac-Man'in aynisi ama daha kucuk ve yone gore donuk.)
void Player::drawClassic(sf::RenderWindow& window) const {
    // Agiz acikligini sin dalgasi ile salindir (0..50 derece)
    float openAmount = (std::sin(animTime_ * 10.f) + 1.f) * 0.5f;
    float mouthAngle = openAmount * 50.f;

    // Yone gore baslangic acisi (dairenin hangi tarafa agiz acacagi)
    float baseAngle = 0.f;
    switch (direction_) {
        case Direction::Right: baseAngle = 0.f;   break;
        case Direction::Down:  baseAngle = 90.f;  break;
        case Direction::Left:  baseAngle = 180.f; break;
        case Direction::Up:    baseAngle = 270.f; break;
        case Direction::None:  baseAngle = 0.f;   break;
    }

    // Govde: sari daire
    sf::CircleShape body(radius_, 32);
    body.setFillColor(Constants::Colors::PACMAN);
    body.setOrigin(radius_, radius_);
    body.setPosition(position_);
    window.draw(body);

    // Agiz: arka plan rengiyle ucgen cizip parca keseriz
    sf::ConvexShape mouth;
    mouth.setPointCount(3);
    mouth.setPoint(0, sf::Vector2f(0.f, 0.f));

    float r = radius_ + 2.f;
    float rad1 = (baseAngle + mouthAngle) * 3.14159265f / 180.f;
    float rad2 = (baseAngle - mouthAngle) * 3.14159265f / 180.f;

    mouth.setPoint(1, sf::Vector2f(std::cos(rad1) * r, std::sin(rad1) * r));
    mouth.setPoint(2, sf::Vector2f(std::cos(rad2) * r, std::sin(rad2) * r));

    mouth.setFillColor(Constants::Colors::BACKGROUND);
    mouth.setPosition(position_);
    window.draw(mouth);
}