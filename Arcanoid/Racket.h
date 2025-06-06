#pragma once
#include "SFML/Graphics.hpp"
#include "Collidable.h"
#include "Defender.h"

class Game;

class Racket : public Attacker {
public:
    explicit Racket() = delete;
    explicit Racket(const Racket& other) = default;
    explicit Racket(Racket&& other) noexcept = default;
    explicit Racket(Game& parent, const float y, const float speed, const sf::Vector2f& size,
        const sf::Color& color = sf::Color::Black);
    virtual ~Racket() = default;
    void draw();
    void handleEvent(const sf::Event& event);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
   
    virtual std::function<void(void)> collideWith(DefenderBall& ball) const override;
    virtual std::function<void(void)> collideWith(DefenderBlock& block) const override;
    virtual std::function<void(void)> collideWith(DefenderBonus& bonus) const override;
    void setSize(const sf::Vector2f& size);
    void setSizeTemporary(const sf::Vector2f& size, float durationMs);
    void updateSizeEffect();

private:
    bool _collideWithBall(const sf::Vector2f& ballBackupPos, const sf::Vector2f& selfBackupPos, DefenderBall& ball) const;
    void _move();
    void _update();
    void _handleWindowCollision();


    Game& _parent;
    sf::RectangleShape _shape;
    float _speedDefault;
    float _speed;
};