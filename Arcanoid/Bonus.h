#pragma once

#include <SFML/Graphics.hpp>
#include "Defender.h"

class Game;

class Bonus : public DefenderBonus {
public:
    explicit Bonus() = delete;
    explicit Bonus(const Bonus& other) = default;
    explicit Bonus(Bonus&& other) = default;
    explicit Bonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
        const float speed, const sf::Color& color);
    virtual ~Bonus() = default;
    void draw();
    // DefenderBonus
    virtual bool isDead() const override;
    virtual void invokeBonusAction() = 0;
    virtual sf::Vector2f getPosition() const override;
    virtual sf::Vector2f getSize() const override;
protected:
    void _move();

    Game& _parent;
    sf::RectangleShape _shape;
    float _speed;
    bool _isDead;
};


class MovingBlockBonus : public Bonus { // Движущиеся блоки
public:
    explicit MovingBlockBonus() = delete;
    explicit MovingBlockBonus(const MovingBlockBonus& other) = default;
    explicit MovingBlockBonus(MovingBlockBonus&& other) = default;
    explicit MovingBlockBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
        const float speed, const sf::Color& color = sf::Color(255, 165, 0, 255));
    virtual ~MovingBlockBonus() = default;
    virtual void invokeBonusAction() override;
};

class SizeBonus : public Bonus { // Увеличение каретки
public:
    explicit SizeBonus() = delete;
    explicit SizeBonus(const SizeBonus& other) = default;
    explicit SizeBonus(SizeBonus&& other) = default;
    explicit SizeBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
        const float speed, const sf::Color& color = sf::Color(255, 182, 193)); 
    virtual ~SizeBonus() = default;
    virtual void invokeBonusAction() override;
};

class StickyBottomBonus : public Bonus { // Дополнительное дно
public:
    explicit StickyBottomBonus() = delete;
    explicit StickyBottomBonus(const StickyBottomBonus& other) = default;
    explicit StickyBottomBonus(StickyBottomBonus&& other) = default;
    explicit StickyBottomBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
        const float speed, const sf::Color& color = sf::Color(171, 12, 166));
    virtual ~StickyBottomBonus() = default;
    virtual void invokeBonusAction() override;
};


class StickyBallBonus : public Bonus { // Прилепание мяча
public:
    explicit StickyBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
        const float speed, const sf::Color& color = sf::Color(135, 206, 250)); 
    virtual ~StickyBallBonus() = default;
    virtual void invokeBonusAction() override;
};
