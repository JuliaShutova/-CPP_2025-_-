#pragma once
#include <SFML/Graphics.hpp>
#include "Collidable.h"
#include "Defender.h"
#include <chrono>

class Game;

class Ball : public DefenderBall, public Attacker {
public:
    explicit Ball() = delete;
    explicit Ball(const Ball& other) = default;
    explicit Ball(Ball&& other) noexcept = default;
    explicit Ball(Game& parent, const sf::Vector2f& velocity, const float radius,
        const int fallScoreIncrease = -15, const sf::Color& color = sf::Color(220, 20, 60));
    virtual ~Ball() = default;
    virtual bool isDead() const;
    void draw();
    // DefenderBall
    virtual sf::Vector2f getPosition() const override;
    virtual float getRadius() const override;
    virtual sf::Vector2f getVelocity() const override;
    virtual void setPosition(const sf::Vector2f& pos) override;
    virtual void setVelocity(const sf::Vector2f& velocity) override;
    virtual void boostUp(const float multiplier, const float timeMs) override;
    // Attacker
    virtual std::function<void(void)> collideWith(DefenderBall& ball) const override;
    virtual std::function<void(void)> collideWith(DefenderBlock& block) const override;
    virtual std::function<void(void)> collideWith(DefenderBonus& bonus) const override;
    
    void handleEvent(const sf::Event& event);
protected:
    void _move();
    void _update();
    virtual void _handleWindowCollision();
    void _handleBoost();

    Game& _parent;
    sf::CircleShape _shape;
    sf::Vector2f _velocity;
    int _fallScoreIncrease;
    std::chrono::time_point<std::chrono::system_clock> _lastBoostTimePoint;
    float _lastBoostDuration;
    float _velocityNormDefault;
    sf::Color _colorDefault;
    bool _boosted;
    bool isMoving() const;
};

