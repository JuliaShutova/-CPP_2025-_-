#include "Game.h"
#include <iostream>

#define GAME_NAME "Arkanoid"

const sf::Vector2f BALL_VEL = sf::Vector2f(0.4f, 0.4f);
constexpr float BALL_RADIUS = 10.0f;
constexpr float RACKET_Y = 0.8f;
constexpr float RACKET_SPEED = 1.0f;
const sf::Vector2f RACKET_SIZE = sf::Vector2f(0.2f, 0.02f);

constexpr float BLOCKS_GRID_Y = 0.1f;
constexpr float BLOCKS_GRID_HEIGHT = 0.3f;
constexpr float BLOCKS_GRID_MARGINX = 0.15f;
const sf::Vector2u BLOCKS_GRID_UV = sf::Vector2u(5, 7);
constexpr unsigned SOFT_BLOCK_HEALTH = 2;
constexpr unsigned BOOST_BLOCK_HEALTH = 1;
constexpr unsigned BONUS_BLOCK_HEALTH = 1;
constexpr float BOOST_BLOCK_MULTIPLIER = 1.5f;
constexpr float BOOST_BLOCK_DURATIONMS = 1000.0f;

const sf::Vector2f BONUS_SIZE = sf::Vector2f(0.02f, 0.02f);
constexpr float BONUS_SPEED = 0.1f;
constexpr unsigned MOVING_BLOCK_HEALTH = 3;
constexpr float MOVING_BLOCK_SPEED = 0.1f;
const std::string ARIAL_FONT_PATH = "Ninjagardenlaser.ttf";

Game::Game() : Game(800, 600) {}

Game::Game(const unsigned winWidth, const unsigned winHeight) :
    _timeDeltaMs(0), _score(0),
    _window(std::make_unique<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), GAME_NAME)),
    _balls({ std::make_shared<Ball>(*this, BALL_VEL, BALL_RADIUS) }),
    _racket(std::make_shared<Racket>(*this, winHeight* RACKET_Y, RACKET_SPEED,
        sf::Vector2f(winWidth* RACKET_SIZE.x, winHeight* RACKET_SIZE.y))),
    _collisionsManager(std::make_unique<CollisionsManager>()),
    _scoreLabel(std::make_unique<Label>(*this, ARIAL_FONT_PATH)),
    _blocksGrid(std::make_unique<BlocksGrid>(
        *this,
        sf::FloatRect(BLOCKS_GRID_MARGINX* winWidth,
            BLOCKS_GRID_Y* winHeight,
            winWidth* (1.0f - 2.0f * BLOCKS_GRID_MARGINX),
            BLOCKS_GRID_HEIGHT* winHeight
        ),
        BLOCKS_GRID_UV,
        SOFT_BLOCK_HEALTH,
        BOOST_BLOCK_HEALTH,
        BONUS_BLOCK_HEALTH,
        MOVING_BLOCK_HEALTH,
        BOOST_BLOCK_MULTIPLIER,
        BOOST_BLOCK_DURATIONMS,
        MOVING_BLOCK_SPEED)) {
    // Инициализация дна
    _stickyBottom.setSize(sf::Vector2f(winWidth, 20)); 
    _stickyBottom.setFillColor(sf::Color(204, 119, 34));
    _stickyBottom.setPosition(0, winHeight - 10);

    _collisionsManager->addAttacker(_balls[0]);
    _collisionsManager->addDefender(_balls[0]);
    _collisionsManager->addAttacker(_racket);
    _collisionsManager->addAttacker(*_blocksGrid);
    _collisionsManager->addDefender(*_blocksGrid);

}

std::unique_ptr<sf::RenderWindow>& Game::getWindow() {
    return _window;
}

std::shared_ptr<Racket> Game::getRacket() const {
    return _racket;
}

float Game::getTimeMsSinceLastFrame() const {
    return _timeDeltaMs;
}


void Game::spawnRandomBonus(const sf::Vector2f& pos) {
    sf::Vector2f bonusSize(
        BONUS_SIZE.x * _window->getSize().x,
        BONUS_SIZE.y * _window->getSize().y
    );
    std::shared_ptr<Bonus> newBonus;
    switch (rand() % 4) {
    case 0:
        newBonus = std::make_shared<MovingBlockBonus>(*this, pos - bonusSize / 2.0f, bonusSize, BONUS_SPEED);
        break;
    case 1:
        newBonus = std::make_shared<SizeBonus>(*this, pos - bonusSize / 2.0f, bonusSize, BONUS_SPEED);
        break;
    case 2:
        newBonus = std::make_shared<StickyBottomBonus>(*this, pos - bonusSize / 2.0f, bonusSize, BONUS_SPEED);
        break;
    case 3:
        newBonus = std::make_shared<StickyBallBonus>(*this, pos - bonusSize / 2.0f, bonusSize, BONUS_SPEED);
        break;

    }
    _bonuses.push_back(newBonus);
    _collisionsManager->addDefender(newBonus);
}

void Game::spawnMovingBlock() {
    auto block = _blocksGrid->spawnMovingBlock();
    if (!block)
        return;
    _collisionsManager->addAttacker(block);
    _collisionsManager->addDefender(block);
}

void Game::increaseScore(const int score) {
    _score += score;
}




void Game::enableStickyBottom() {
    _stickyBottomEnabled = true;
}

bool Game::isStickyBottomEnabled() const {
    return _stickyBottomEnabled;
}

void Game::disableStickyBottom() {
    _stickyBottomEnabled = false;
}

int Game::run() {
    _lastTimePoint = std::chrono::high_resolution_clock::now();
    while (_window->isOpen()) {
        _refreshTimeDeltaMs(); // Обновляем время с последнего кадра

        // Обработка событий
        sf::Event event;
        while (_window->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                _window->close(); // Закрытие окна
                return 0;

            case sf::Event::MouseButtonReleased: // Отладочное событие для создания движущегося блока
                spawnMovingBlock();
                break;

            default:
                _racket->handleEvent(event); // Передача событий ракетке
                for (auto& ball : _balls) { // Обработка событий для шариков
                    ball->handleEvent(event);
                }
                break;
               
            }
        }

        // Очистка 
        _window->clear(sf::Color(253, 244, 227));

        // Обработка коллизий
        _collisionsManager->handleCollisions();

        // Обновление текста 
        _updateScoreLabel();

        // Отрисовка 
        _blocksGrid->draw(); // Блоки
        _racket->draw();     // Ракетка
        for (auto& ball : _balls) {
            ball->draw();    // Шарики
        }
        
        for (auto& bonus : _bonuses) {
            bonus->draw();   // Бонусы
        }


        _scoreLabel->draw(); // Счет

        if (_stickyBottomEnabled) {
            _window->draw(_stickyBottom);
        }

        // Отображение кадра
        _window->display();

        // Удаление  объектов
        _blocksGrid->destroyDeadBlocks(); // блоков
        _destroyDeadBalls();              //  шариков
        _destroyDeadBonuses();            // собранных бонусов

        if (_stickyBallEnabled) { 
            auto now = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _stickyBallTimePoint).count();
            if (elapsed > _stickyBallDurationMs) {
                disableStickyBall();
            }
        } 

        // Обработка столкновений 
        for (auto& ball : _balls) {
           
            if (_stickyBottomEnabled && ball->getPosition().y + ball->getRadius() * 2 >= _stickyBottom.getPosition().y) {
                // Устанавливаем новую позицию шарика
                ball->setPosition(sf::Vector2f(
                    ball->getPosition().x,
                    _stickyBottom.getPosition().y - ball->getRadius() * 2
                ));

                // Меняем направление скорости по оси Y
                ball->setVelocity(sf::Vector2f(
                    ball->getVelocity().x,
                    -ball->getVelocity().y
                ));

                disableStickyBottom();
            }
        }

    }
    return 0;
}

void Game::_refreshTimeDeltaMs() {
    auto now = std::chrono::high_resolution_clock::now();
    _timeDeltaMs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _lastTimePoint).count() / 1e6f;
    _lastTimePoint = now;
}

void Game::_destroyDeadBalls() {
    auto ballIt = _balls.begin();
    while (ballIt != _balls.end()) {
        if ((*ballIt)->isDead())
            ballIt = _balls.erase(ballIt);
        else
            ballIt++;
    }
}

void Game::_destroyDeadBonuses() {
    auto bonusIt = _bonuses.begin();
    while (bonusIt != _bonuses.end()) {
        if ((*bonusIt)->isDead())
            bonusIt = _bonuses.erase(bonusIt);
        else
            bonusIt++;
    }
}

void Game::_updateScoreLabel() {
    _scoreLabel->setText(L"Score: " + std::to_wstring(_score));
}

void Game::enableStickyBall() {
    _stickyBallEnabled = true;
    _stickyBallTimePoint = std::chrono::system_clock::now();
}

bool Game::isStickyBallEnabled() const {
    return _stickyBallEnabled;
}

void Game::disableStickyBall() {
    _stickyBallEnabled = false;
}


