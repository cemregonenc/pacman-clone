#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Maze.h"

// Hayalet sinifi.
// 2 farkli davranisi olacak (proje sarti):
//   Personality::Chaser   -> BFS ile en kisa yoldan Pac-Man'i takip (Blinky)
//   Personality::Predict  -> Pac-Man'in onune dogru hedef secer (Pinky)
class Ghost {
public:
    enum class Direction { None, Up, Down, Left, Right };
    enum class Personality { Chaser, Predict };
    enum class Mode { Chase, Frightened, Eaten };

    Ghost(Personality p, sf::Color color, sf::Vector2f startPos);

    void update(sf::Time deltaTime, const Maze& maze, sf::Vector2f playerPos,
                Direction playerDir);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f position() const { return position_; }
    void         setMode(Mode m)  { mode_ = m; }
    Mode         mode() const     { return mode_; }
    void         reset();

private:
    void chooseDirection(const Maze& maze, sf::Vector2f playerPos,
                         Direction playerDir);

    sf::Vector2i currentTile() const;
    bool         atTileCenter() const;
    sf::Vector2f directionVector(Direction d) const;
    bool         canMoveInDirection(Direction d, const Maze& maze) const;
    // BFS ile labirentte en kisa yolu bulup ilk adimi dondurur
    Direction    bfsNextStep(const Maze& maze,
                             sf::Vector2i from,
                             sf::Vector2i target) const;
    // Greedy: en yakindaki hedefe goturen yonu sec (BFS'ten daha basit)
    Direction    greedyNextStep(const Maze& maze,
                                sf::Vector2i from,
                                sf::Vector2i target) const;
    // Uyeler
    Personality  personality_;
    sf::Color    color_;
    sf::Vector2f position_;
    sf::Vector2f startPos_;
    Direction    direction_;
    Direction    previousDirection_;
    // Pinky takilma tespiti icin
    sf::Vector2i lastTile_;
    int          stuckCounter_;
    float        speed_;
    float        radius_;
    Mode         mode_;
    float        animTime_;
};