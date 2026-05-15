#include "Player.h"
#include <cmath>


Player::Player()
    : position_(14.f * Constants::TILE_SIZE,
                23.f * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f)
    , direction_(Direction::Right)
    , desiredDirection_(Direction::Right)
    , animTime_(0.f)
    , radius_(Constants::TILE_SIZE * 0.45f)
    , speed_(Constants::TILE_SIZE * 5.f)
    , useCustomImage_(false)
{
    // Ozel fotograf var mi? Varsa yukle ve "kullan" bayragini ac
    if (customTexture_.loadFromFile("assets/images/custom_pacman.png")) {
        customTexture_.setSmooth(true);
        customSprite_.setTexture(customTexture_);

        // Sprite'i Pac-Man cap'ine olcekle
        sf::Vector2u sz = customTexture_.getSize();
        float scale = (radius_ * 2.f) / static_cast<float>(sz.x);
        customSprite_.setScale(scale, scale);

        // Origin merkez: rotate ettiginde dogru donsun
        customSprite_.setOrigin(sz.x / 2.f, sz.y / 2.f);

        useCustomImage_ = true;
    }
}
    



// Yon tuslarini oku
void Player::handleInput(const Maze& /*maze*/) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        desiredDirection_ = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        desiredDirection_ = Direction::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        desiredDirection_ = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        desiredDirection_ = Direction::Right;
}


void Player::update(sf::Time deltaTime, const Maze& maze) {
    animTime_ += deltaTime.asSeconds();

    // Tile merkezinde yon kararlarini ver
    if (atTileCenter()) {
        // 1) Istenen yone donmek mumkun mu? Mumkunse don
        if (desiredDirection_ != direction_ &&
            canMoveInDirection(desiredDirection_, maze)) {
            direction_ = desiredDirection_;
        }
        // 2) Mevcut yonde de duvar varsa dur
        if (!canMoveInDirection(direction_, maze)) {
            // Pozisyonu tam tile merkezine kilitle (kayma olmasin)
            sf::Vector2i tile = currentTile();
            position_.x = tile.x * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
            position_.y = tile.y * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
            return;
        }
    }

    // Hareket et
    sf::Vector2f delta = directionVector(direction_) * speed_ * deltaTime.asSeconds();
    position_ += delta;

    // Yan tunel
    float maxX = Constants::MAZE_COLS * Constants::TILE_SIZE;
    if (position_.x < -radius_) position_.x = maxX + radius_;
    if (position_.x > maxX + radius_) position_.x = -radius_;
}


sf::Vector2i Player::currentTile() const {
    int col = static_cast<int>(position_.x / Constants::TILE_SIZE);
    int row = static_cast<int>(position_.y / Constants::TILE_SIZE);
    return { col, row };
}


bool Player::atTileCenter() const {
    float cx = currentTile().x * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
    float cy = currentTile().y * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
    const float tolerance = 2.f;
    return std::abs(position_.x - cx) < tolerance &&
           std::abs(position_.y - cy) < tolerance;
}


sf::Vector2f Player::directionVector(Direction d) const {
    switch (d) {
        case Direction::Up:    return { 0.f, -1.f };
        case Direction::Down:  return { 0.f,  1.f };
        case Direction::Left:  return {-1.f,  0.f };
        case Direction::Right: return { 1.f,  0.f };
        default:               return { 0.f,  0.f };
    }
}


// Bulundugumuz tile'dan o yondeki KOMSU tile duvar mi?
bool Player::canMoveInDirection(Direction d, const Maze& maze) const {
    sf::Vector2i tile = currentTile();
    sf::Vector2f v = directionVector(d);
    int nextCol = tile.x + static_cast<int>(v.x);
    int nextRow = tile.y + static_cast<int>(v.y);
    return !maze.isWall(nextCol, nextRow);
}


void Player::draw(sf::RenderWindow& window) const {
    if (useCustomImage_)
        drawCustomImage(window);
    else
        drawClassic(window);
}

// Ozel fotograflı Pac-Man:
//   - sprite yon vektorune gore donduruluyor
//   - uzerine arka plan renginde ucgen ile "agiz" parcasi kesiliyor
void Player::drawCustomImage(sf::RenderWindow& window) const {
    sf::Sprite spr = customSprite_;     // const fonksiyonda kopya alip oynuyoruz
    spr.setPosition(position_);

    float rotation = 0.f;
    switch (direction_) {
        case Direction::Right: rotation = 0.f;   break;
        case Direction::Down:  rotation = 90.f;  break;
        case Direction::Left:  rotation = 180.f; break;
        case Direction::Up:    rotation = 270.f; break;
        case Direction::None:  rotation = 0.f;   break;
    }
    spr.setRotation(rotation);
    window.draw(spr);

    // Agiz: arka plan renginde ucgen ile bir parca kes
    float openAmount = (std::sin(animTime_ * 10.f) + 1.f) * 0.5f;
    float mouthAngle = openAmount * 50.f;

    sf::ConvexShape mouth;
    mouth.setPointCount(3);
    mouth.setPoint(0, sf::Vector2f(0.f, 0.f));

    float r = radius_ + 2.f;
    float rad1 = mouthAngle * 3.14159265f / 180.f;
    float rad2 = -mouthAngle * 3.14159265f / 180.f;

    mouth.setPoint(1, sf::Vector2f(std::cos(rad1) * r, std::sin(rad1) * r));
    mouth.setPoint(2, sf::Vector2f(std::cos(rad2) * r, std::sin(rad2) * r));

    mouth.setFillColor(Constants::Colors::BACKGROUND);
    mouth.setPosition(position_);
    mouth.setRotation(rotation);   // Agiz da yone gore donsun
    window.draw(mouth);
}


void Player::drawClassic(sf::RenderWindow& window) const {
    float openAmount = (std::sin(animTime_ * 10.f) + 1.f) * 0.5f;
    float mouthAngle = openAmount * 50.f;

    float baseAngle = 0.f;
    switch (direction_) {
        case Direction::Right: baseAngle = 0.f;   break;
        case Direction::Down:  baseAngle = 90.f;  break;
        case Direction::Left:  baseAngle = 180.f; break;
        case Direction::Up:    baseAngle = 270.f; break;
        case Direction::None:  baseAngle = 0.f;   break;
    }

    sf::CircleShape body(radius_, 32);
    body.setFillColor(Constants::Colors::PACMAN);
    body.setOrigin(radius_, radius_);
    body.setPosition(position_);
    window.draw(body);

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
// Pac-Man'i baslangic konumuna gonder (can kaybinda cagrilir)
void Player::reset() {
    position_ = sf::Vector2f(
        14.f * Constants::TILE_SIZE,
        23.f * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f
    );
    direction_ = Direction::Right;
    desiredDirection_ = Direction::Right;
}