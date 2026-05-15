#include "Maze.h"

// Klasik Pac-Man labirenti (28 sutun x 31 satir).
// Simetrik tasarim: sol yari ile sag yari ayna goruntusu, ortada hayalet odasi.
//
// Lejant:
//   # duvar     . nokta      o guc meyvesi
//   ' ' bosluk  - hayalet kapisi
const std::vector<std::string> Maze::RAW_MAP = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o####.#####.##.#####.####o#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "######.##### ## #####.######",
    "######.##          ##.######",
    "######.## ######## ##.######",
    "######.## #      # ##.######",
    "          #      #          ",
    "######.## #      # ##.######",
    "######.## ######## ##.######",
    "######.##          ##.######",
    "######.## ######## ##.######",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#.####.#####.##.#####.####.#",
    "#o..##................##..o#",
    "###.##.##.########.##.##.###",
    "###.##.##.########.##.##.###",
    "#......##....##....##......#",
    "#.##########.##.##########.#",
    "#.##########.##.##########.#",
    "#..........................#",
    "############################"
};


Maze::Maze() : pelletCount_(0) {
    grid_.resize(Constants::MAZE_ROWS,
                 std::vector<Tile>(Constants::MAZE_COLS, Tile::Empty));

    for (int row = 0; row < Constants::MAZE_ROWS; ++row) {
        const std::string& line = RAW_MAP[row];
        for (int col = 0; col < Constants::MAZE_COLS; ++col) {
            Tile t = charToTile(line[col]);
            grid_[row][col] = t;
            if (t == Tile::Pellet || t == Tile::PowerPellet) {
                ++pelletCount_;
            }
        }
    }
}


// Karakter -> Tile cevirimi
Maze::Tile Maze::charToTile(char c) {
    switch (c) {
        case '#': return Tile::Wall;
        case '.': return Tile::Pellet;
        case 'o': return Tile::PowerPellet;
        case '-': return Tile::GhostGate;
        default:  return Tile::Empty;
    }
}


// Belirli bir hucredeki tile'i dondur
Maze::Tile Maze::at(int col, int row) const {
    if (!isInBounds(col, row)) return Tile::Empty;
    return grid_[row][col];
}


bool Maze::isWall(int col, int row) const {
    return at(col, row) == Tile::Wall;
}


bool Maze::isInBounds(int col, int row) const {
    return col >= 0 && col < Constants::MAZE_COLS
        && row >= 0 && row < Constants::MAZE_ROWS;
}


// Labirenti ciz: duvarlar, noktalar, guc meyveleri, hayalet kapisi.
// pelletPhase parametresi yanip sonme animasyonu icin (Game'den geliyor).
void Maze::draw(sf::RenderWindow& window) const {
    const float ts = static_cast<float>(Constants::TILE_SIZE);

    for (int row = 0; row < Constants::MAZE_ROWS; ++row) {
        for (int col = 0; col < Constants::MAZE_COLS; ++col) {
            Tile t = grid_[row][col];
            float x = col * ts;
            float y = row * ts;

            if (t == Tile::Wall) {
                sf::RectangleShape body(sf::Vector2f(ts, ts));
                body.setPosition(x, y);
                body.setFillColor(Constants::Colors::WALL);
                window.draw(body);
            }
            else if (t == Tile::Pellet) {
                // Kucuk yenebilir nokta (tile merkezinde)
                sf::CircleShape dot(ts * 0.10f, 8);
                dot.setFillColor(Constants::Colors::PELLET);
                dot.setOrigin(ts * 0.10f, ts * 0.10f);
                dot.setPosition(x + ts / 2.f, y + ts / 2.f);
                window.draw(dot);
            }
            else if (t == Tile::PowerPellet) {
                // Buyuk guc meyvesi (sari)
                sf::CircleShape big(ts * 0.30f, 16);
                big.setFillColor(Constants::Colors::POWER_PELLET);
                big.setOrigin(ts * 0.30f, ts * 0.30f);
                big.setPosition(x + ts / 2.f, y + ts / 2.f);
                window.draw(big);
            }
            else if (t == Tile::GhostGate) {
                sf::RectangleShape gate(sf::Vector2f(ts, 4.f));
                gate.setPosition(x, y + ts / 2.f - 2.f);
                gate.setFillColor(Constants::Colors::GHOST_PINK);
                window.draw(gate);
            }
        }
    }
}
// Belirtilen tile'da pellet varsa yer, skor doner. Yoksa 0 doner.
int Maze::eatPelletAt(int col, int row) {
    if (!isInBounds(col, row)) return 0;

    Tile t = grid_[row][col];
    if (t == Tile::Pellet) {
        grid_[row][col] = Tile::Empty;
        --pelletCount_;
        return 10;          // Klasik Pac-Man: pellet = 10 puan
    }
    if (t == Tile::PowerPellet) {
        grid_[row][col] = Tile::Empty;
        --pelletCount_;
        return 50;          // Power pellet = 50 puan
    }
    return 0;
}