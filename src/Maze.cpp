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


Maze::Maze() {
    // Karakter haritasini 2D Tile dizisine cevir.
    // Boyle bir kez yapip sonra hizli arama yapariz; her frame parse etmeyiz.
    grid_.resize(Constants::MAZE_ROWS,
                 std::vector<Tile>(Constants::MAZE_COLS, Tile::Empty));

    for (int row = 0; row < Constants::MAZE_ROWS; ++row) {
        const std::string& line = RAW_MAP[row];
        for (int col = 0; col < Constants::MAZE_COLS; ++col) {
            grid_[row][col] = charToTile(line[col]);
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


// Labirenti ciz: duvarlar (mavi blok + acik mavi parlama cercevesi).
// Noktalar ve guc meyveleri Adim 4'te eklenecek.
void Maze::draw(sf::RenderWindow& window) const {
    const float ts = static_cast<float>(Constants::TILE_SIZE);

    for (int row = 0; row < Constants::MAZE_ROWS; ++row) {
        for (int col = 0; col < Constants::MAZE_COLS; ++col) {
            Tile t = grid_[row][col];
            float x = col * ts;
            float y = row * ts;

            if (t == Tile::Wall) {
                // Duvar govdesi: mavi dolgu
                sf::RectangleShape body(sf::Vector2f(ts, ts));
                body.setPosition(x, y);
                body.setFillColor(Constants::Colors::WALL);
                window.draw(body);

                // Parlama cercevesi: acik mavi, ince kenarlik
                // (klasik Pac-Man duvarlarinin neon hissi)
                sf::RectangleShape glow(sf::Vector2f(ts - 4.f, ts - 4.f));
                glow.setPosition(x + 2.f, y + 2.f);
                glow.setFillColor(sf::Color::Transparent);
                glow.setOutlineColor(Constants::Colors::WALL_GLOW);
                glow.setOutlineThickness(1.f);
                window.draw(glow);
            }
            else if (t == Tile::GhostGate) {
                // Hayalet kapisi: ince pembe cubuk (ust ortada)
                sf::RectangleShape gate(sf::Vector2f(ts, 4.f));
                gate.setPosition(x, y + ts / 2.f - 2.f);
                gate.setFillColor(Constants::Colors::GHOST_PINK);
                window.draw(gate);
            }
            // Pellet / PowerPellet / Empty -> Adim 4'te
        }
    }
}