#include "Game.h"
#include <cmath>
#include <iostream>

// Sabit zaman adimi: her frame 1/60 saniye surer.
// Bu sayede oyun hizli/yavas bilgisayarda ayni hizda kosuyor.
const sf::Time Game::TIME_PER_FRAME = sf::seconds(1.f / Constants::FPS);


Game::Game()
    : window_(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT),
              "Pac-Man Klonu - YZM104",
              sf::Style::Titlebar | sf::Style::Close)
    , fontLoaded_(false)
    , state_(Constants::GameState::Menu)
    , timeSinceLastUpdate_(sf::Time::Zero)
    , menuAnimTime_(0.f)
{
    window_.setFramerateLimit(Constants::FPS);
    fontLoaded_ = font_.loadFromFile("assets/fonts/Jersey10.ttf");
    if (!fontLoaded_) {
        std::cerr << "[Uyari] Font bulunamadi.\n";
    }
}


// Ana oyun dongusu: input -> update -> render
// Birikmis zaman bir frame'i asana kadar update yapariz; takilan
// frame'leri telafi eder, mantik FPS'ten bagimsiz kalir.
void Game::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate_ += dt;

        while (timeSinceLastUpdate_ >= TIME_PER_FRAME) {
            timeSinceLastUpdate_ -= TIME_PER_FRAME;
            processEvents();
            update(TIME_PER_FRAME);
        }
        render();
    }
}


// Klavye girdileri ve pencere kapatma
void Game::processEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    window_.close();
                    break;
                case sf::Keyboard::Enter:
                    if (state_ == Constants::GameState::Menu)
                        state_ = Constants::GameState::Playing;
                    break;
                case sf::Keyboard::P:
                    // Playing <-> Paused arasi gecis
                    if (state_ == Constants::GameState::Playing)
                        state_ = Constants::GameState::Paused;
                    else if (state_ == Constants::GameState::Paused)
                        state_ = Constants::GameState::Playing;
                    break;
                default: break;
            }
        }
    }
}


// Adim 1'de sadece menu animasyon zamanini ilerletiyoruz.
// Adim 2'den itibaren burada Pac-Man hareketi, hayalet AI'i, carpisma
// kontrolleri yapilacak.
void Game::update(sf::Time deltaTime) {
    menuAnimTime_ += deltaTime.asSeconds();
}


// Duruma gore ilgili ekrani ciz
void Game::render() {
    window_.clear(Constants::Colors::BACKGROUND);
    switch (state_) {
        case Constants::GameState::Menu:     renderMenu();     break;
        case Constants::GameState::Playing:  renderPlaying();  break;
        case Constants::GameState::Paused:   renderPaused();   break;
        case Constants::GameState::GameOver: renderGameOver(); break;
        default: break;
    }
    window_.display();
}


void Game::renderMenu() {
    const float cx = Constants::WINDOW_WIDTH  / 2.f;
    const float cy = Constants::WINDOW_HEIGHT / 2.f;

    // Agiz acikligini sin dalgasi ile 0..60 derece arasinda salindir
    float openAmount = (std::sin(menuAnimTime_ * 6.f) + 1.f) * 0.5f;
    float mouthAngle = openAmount * 60.f;

    sf::CircleShape pac(70.f, 60);
    pac.setFillColor(Constants::Colors::PACMAN);
    pac.setOrigin(70.f, 70.f);
    pac.setPosition(cx, cy - 40.f);
    window_.draw(pac);

    // Agiz efekti: dairenin uzerine arka plan renginde bir ucgen cizip
    // o parcayi "kesiyoruz". Ucgenin acisi animasyonla degisir.
    sf::ConvexShape mouth;
    mouth.setPointCount(3);
    mouth.setPoint(0, sf::Vector2f(0.f, 0.f));
    float rad = mouthAngle * 3.14159265f / 180.f;
    float r = 72.f;
    mouth.setPoint(1, sf::Vector2f(r,  std::sin(rad) * r));
    mouth.setPoint(2, sf::Vector2f(r, -std::sin(rad) * r));
    mouth.setFillColor(Constants::Colors::BACKGROUND);
    mouth.setPosition(cx, cy - 40.f);
    window_.draw(mouth);

    sf::CircleShape eye(6.f);
    eye.setFillColor(sf::Color::Black);
    eye.setPosition(cx - 12.f, cy - 90.f);
    window_.draw(eye);

    if (!fontLoaded_) return;

    sf::Text title("PAC-MAN", font_, 72);
    title.setFillColor(Constants::Colors::UI_HIGHLIGHT);
    title.setOutlineColor(Constants::Colors::WALL);
    title.setOutlineThickness(3.f);
    centerText(title, cx, 100.f);
    window_.draw(title);

    sf::Text subtitle("KLONU", font_, 36);
    subtitle.setFillColor(Constants::Colors::UI_TEXT);
    centerText(subtitle, cx, 160.f);
    window_.draw(subtitle);

    // Yanip sonen "ENTER" prompt'u: alpha degerini sin ile salindiririz
    sf::Text prompt("ENTER ile Basla", font_, 28);
    int alpha = static_cast<int>((std::sin(menuAnimTime_ * 4.f) + 1.f) * 127.f);
    prompt.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
    centerText(prompt, cx, cy + 130.f);
    window_.draw(prompt);

    sf::Text esc("ESC: Cikis  |  P: Duraklat", font_, 16);
    esc.setFillColor(sf::Color(180, 180, 180));
    centerText(esc, cx, Constants::WINDOW_HEIGHT - 30.f);
    window_.draw(esc);
}


void Game::renderPlaying() {
    if (!fontLoaded_) return;
    sf::Text msg("Asama 1 tamam: pencere ve oyun dongusu calisiyor.\n"
                 "Sonraki adim: labirenti cizmek.",
                 font_, 14);
    msg.setFillColor(Constants::Colors::UI_TEXT);
    msg.setPosition(40.f, 40.f);
    window_.draw(msg);
}


// Pause ekrani: alttaki oyunu ciz, ustune yari saydam overlay koy
void Game::renderPaused() {
    renderPlaying();
    if (!fontLoaded_) return;

    sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH,
                                            Constants::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window_.draw(overlay);

    sf::Text txt("DURAKLATILDI\nP: Devam", font_, 24);
    txt.setFillColor(Constants::Colors::UI_HIGHLIGHT);
    centerText(txt, Constants::WINDOW_WIDTH / 2.f,
                    Constants::WINDOW_HEIGHT / 2.f);
    window_.draw(txt);
}


void Game::renderGameOver() {
    if (!fontLoaded_) return;
    sf::Text txt("GAME OVER", font_, 36);
    txt.setFillColor(Constants::Colors::GHOST_RED);
    centerText(txt, Constants::WINDOW_WIDTH / 2.f,
                    Constants::WINDOW_HEIGHT / 2.f);
    window_.draw(txt);
}


// SFML Text varsayilan olarak sol-ust kosesinden konumlanir.
// Origin'i merkeze tasiyarak gercek anlamda ortalamis oluyoruz.
void Game::centerText(sf::Text& text, float x, float y) {
    auto b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    text.setPosition(x, y);
}