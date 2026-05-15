#include "Ghost.h"
#include <cmath>
#include <queue>
#include <vector>


Ghost::Ghost(Personality p, sf::Color color, sf::Vector2f startPos)
    : personality_(p)
    , color_(color)
    , position_(startPos)
    , startPos_(startPos)
    , direction_(Direction::Right)
    , speed_(Constants::TILE_SIZE * 4.f)
    , radius_(Constants::TILE_SIZE * 0.45f)
    , mode_(Mode::Chase)
    , animTime_(0.f)
{
    // Baslangic pozisyonunu tile merkezine snap'le
    int col = static_cast<int>(startPos.x / Constants::TILE_SIZE);
    int row = static_cast<int>(startPos.y / Constants::TILE_SIZE);
    position_.x = col * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
    position_.y = row * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;

}


void Ghost::update(sf::Time deltaTime, const Maze& maze,
                   sf::Vector2f playerPos, Direction playerDir) {
    animTime_ += deltaTime.asSeconds();



    if (atTileCenter()) {
        chooseDirection(maze, playerPos, playerDir);

        if (!canMoveInDirection(direction_, maze)) {
            sf::Vector2i tile = currentTile();
            position_.x = tile.x * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
            position_.y = tile.y * Constants::TILE_SIZE + Constants::TILE_SIZE / 2.f;
            return;
        }
    }

    sf::Vector2f delta = directionVector(direction_) * speed_ * deltaTime.asSeconds();
    position_ += delta;

    // Yan tunel sarmalamasi
    float maxX = Constants::MAZE_COLS * Constants::TILE_SIZE;
    if (position_.x < -radius_)         position_.x = maxX + radius_;
    if (position_.x > maxX + radius_)   position_.x = -radius_;

    float maxY = Constants::MAZE_ROWS * Constants::TILE_SIZE;
    if (position_.y < 0)    position_.y = Constants::TILE_SIZE / 2.f;
    if (position_.y > maxY) position_.y = maxY - Constants::TILE_SIZE / 2.f;
}


void Ghost::draw(sf::RenderWindow& window) const {
    const float r = radius_;

    sf::Color bodyColor = (mode_ == Mode::Frightened)
                          ? Constants::Colors::GHOST_FRIGHTENED
                          : color_;

    // Kafa
    sf::CircleShape head(r, 24);
    head.setFillColor(bodyColor);
    head.setOrigin(r, r);
    head.setPosition(position_);
    window.draw(head);

    // Govde
    sf::RectangleShape body(sf::Vector2f(r * 2.f, r));
    body.setFillColor(bodyColor);
    body.setOrigin(r, 0.f);
    body.setPosition(position_.x, position_.y);
    window.draw(body);

    // Etek
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

    // Gozler
    sf::Vector2f pupilOffset = directionVector(direction_) * (r * 0.15f);

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


// Kisiilige gore hedef tile sec ve BFS ile ilk adimi bul
void Ghost::chooseDirection(const Maze& maze, sf::Vector2f playerPos,
                            Direction playerDir) {
    sf::Vector2i playerTile{
        static_cast<int>(playerPos.x / Constants::TILE_SIZE),
        static_cast<int>(playerPos.y / Constants::TILE_SIZE)
    };

    sf::Vector2i target = playerTile;

    if (personality_ == Personality::Predict) {
        sf::Vector2f v = directionVector(playerDir);
        target.x += static_cast<int>(v.x) * 4;
        target.y += static_cast<int>(v.y) * 4;
    }

    if (mode_ == Mode::Frightened) {
        sf::Vector2i myTile = currentTile();
        target.x = myTile.x - (playerTile.x - myTile.x);
        target.y = myTile.y - (playerTile.y - myTile.y);
    }

    Direction next = bfsNextStep(maze, currentTile(), target);
    if (next != Direction::None) {
        direction_ = next;
    }
}


// BFS algoritmasi: en kisa yoldaki ilk adimi dondurur
Ghost::Direction Ghost::bfsNextStep(const Maze& maze,
                                    sf::Vector2i from,
                                    sf::Vector2i target) const {
    const int W = Constants::MAZE_COLS;
    const int H = Constants::MAZE_ROWS;

    target.x = std::max(0, std::min(W - 1, target.x));
    target.y = std::max(0, std::min(H - 1, target.y));

    if (target.x == from.x && target.y == from.y) {
        return direction_;
    }

    std::vector<std::vector<bool>> visited(H, std::vector<bool>(W, false));
    std::vector<std::vector<sf::Vector2i>> parent(
        H, std::vector<sf::Vector2i>(W, {-1, -1})
    );

    const int dx[4] = {  0,  0, -1,  1 };
    const int dy[4] = { -1,  1,  0,  0 };

    std::queue<sf::Vector2i> q;
    q.push(from);
    visited[from.y][from.x] = true;

    bool found = false;
    while (!q.empty()) {
        sf::Vector2i cur = q.front();
        q.pop();

        if (cur.x == target.x && cur.y == target.y) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (nx < 0) nx = W - 1;
            if (nx >= W) nx = 0;

            if (ny < 0 || ny >= H) continue;
            if (visited[ny][nx]) continue;
            if (maze.isWall(nx, ny)) continue;

            visited[ny][nx] = true;
            parent[ny][nx] = cur;
            q.push({nx, ny});
        }
    }

    if (!found) return Direction::None;

    sf::Vector2i step = target;
    while (!(parent[step.y][step.x].x == from.x &&
             parent[step.y][step.x].y == from.y)) {
        sf::Vector2i p = parent[step.y][step.x];
        if (p.x == -1) return Direction::None;
        step = p;
    }

    int ddx = step.x - from.x;
    int ddy = step.y - from.y;

    if (ddx ==  (W - 1)) ddx = -1;
    if (ddx == -(W - 1)) ddx =  1;

    if (ddx ==  1) return Direction::Right;
    if (ddx == -1) return Direction::Left;
    if (ddy ==  1) return Direction::Down;
    if (ddy == -1) return Direction::Up;
    return Direction::None;
}