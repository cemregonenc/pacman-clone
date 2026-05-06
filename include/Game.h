#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

// Oyunun ana sinifi: pencereyi tutar, oyun dongusunu yonetir,
// durum makinesini (Menu / Playing / Paused / GameOver) isler.
class Game {
public:
    Game();
    void run();

private:
    // Klasik dongu adimlari: input -> update -> render
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    // Duruma gore cizim
    void renderMenu();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();

    // Yardimci
    void centerText(sf::Text& text, float x, float y);

    // Uyeler
    sf::RenderWindow      window_;
    sf::Font              font_;
    bool                  fontLoaded_;
    Constants::GameState  state_;

    // Sabit zaman adimi icin
    sf::Time              timeSinceLastUpdate_;
    static const sf::Time TIME_PER_FRAME;

    // Menu animasyon zamani
    float                 menuAnimTime_;
};