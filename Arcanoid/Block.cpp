#define _USE_MATH_DEFINES

#include "Block.h"
#include "Game.h"
#include <cmath>
#include <iostream> //debugging

constexpr float OUTLINE_THICKNESS_RATIO = 0.05f; 

Block::Block(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Color& color) :
    _parent(parent) {
    _shape.setPosition(pos);
    _shape.setSize(size);
    _shape.setFillColor(color);
    _shape.setOutlineColor(sf::Color::Black);
    const float outlineThickness = OUTLINE_THICKNESS_RATIO * (size.x > size.y ? size.y : size.x);
    _shape.setOutlineThickness(-round(outlineThickness));
}

void Block::draw() {
    _parent.getWindow()->draw(_shape);
}

void Block::setSpeed(const float speed) {}

float Block::getSpeed() const {
    return 0.0f;
}

void Block::setPosition(const sf::Vector2f& pos) {}

bool Block::isDead() const {
    return false;
}

void Block::damage() {}

sf::Vector2f Block::getPosition() const {
    return _shape.getPosition();
}

sf::Vector2f Block::getSize() const {
    return _shape.getSize();
}

std::function<void(void)> Block::collideWith(DefenderBall& ball) const {
    const sf::Vector2f pos = ball.getPosition();
    const sf::Vector2f racketPos = _shape.getPosition();
    return [=, &ball](void) -> void {
        _collideWithBall(pos, racketPos, ball);
        };
}

std::function<void(void)> Block::collideWith(DefenderBlock& block) const {
    if (&block == this)
        return [](void) -> void { return; };
    sf::Vector2f otherPos = block.getPosition();
    sf::Vector2f myPos = getPosition();
    sf::Vector2f otherSize = block.getSize();
    sf::Vector2f mySize = getSize();
    sf::FloatRect otherRect = sf::FloatRect(otherPos, otherSize);
    sf::FloatRect myRect = sf::FloatRect(myPos, mySize);
    float otherSpeed = block.getSpeed();
    float mySpeed = getSpeed();
    if (!myRect.intersects(otherRect))
        return [](void) -> void { return; };
    if (otherSpeed * mySpeed > 0)
        return [](void) -> void { return; };
    return [=, &block](void) -> void {
        if (otherSpeed > 0)
            block.setPosition(sf::Vector2f(myPos.x - otherSize.x, otherPos.y));
        else
            block.setPosition(sf::Vector2f(myPos.x + mySize.x, otherPos.y));
        block.setSpeed(-otherSpeed);
        };
}

std::function<void(void)> Block::collideWith(DefenderBonus& bonus) const {
    return [](void) -> void { return; };
}

bool Block::_collideWithBall(const sf::Vector2f& ballBackupPos, const sf::Vector2f& selfBackupPos, DefenderBall& ball) const {
    sf::Vector2f pos = ballBackupPos;
    sf::Vector2f blockPos = selfBackupPos;
    sf::Vector2f blockSize = _shape.getSize();
    sf::Vector2f velocity = ball.getVelocity();
    float radius = ball.getRadius();
    sf::Vector2f midPoint = pos + sf::Vector2f(radius, radius);
    
    sf::FloatRect top = sf::FloatRect(
        blockPos - sf::Vector2f(0, radius),
        sf::Vector2f(blockSize.x, radius)
    );
    sf::FloatRect bot = sf::FloatRect(
        blockPos + sf::Vector2f(0, blockSize.y),
        sf::Vector2f(blockSize.x, radius)
    );
    sf::FloatRect left = sf::FloatRect(
        blockPos - sf::Vector2f(radius, 0),
        sf::Vector2f(radius, blockSize.y)
    );
    sf::FloatRect right = sf::FloatRect(
        blockPos + sf::Vector2f(blockSize.x, 0),
        sf::Vector2f(radius, blockSize.y)
    );
    
    if (top.contains(midPoint)) {
        ball.setPosition(sf::Vector2f(pos.x, top.top - radius));
        if (velocity.y > 0)
            ball.setVelocity(sf::Vector2f(velocity.x, -velocity.y));
        return true;
    }
    if (bot.contains(midPoint)) {
        ball.setPosition(sf::Vector2f(pos.x, bot.top));
        if (velocity.y < 0)
            ball.setVelocity(sf::Vector2f(velocity.x, -velocity.y));
        return true;
    }
    if (left.contains(midPoint)) {
        ball.setPosition(sf::Vector2f(left.left - radius, pos.y));
        if (velocity.x > 0)
            ball.setVelocity(sf::Vector2f(-velocity.x, velocity.y));
        return true;
    }
    if (right.contains(midPoint)) {
        ball.setPosition(sf::Vector2f(right.left, pos.y));
        if (velocity.x < 0)
            ball.setVelocity(sf::Vector2f(-velocity.x, velocity.y));
        return true;
    }
    
    sf::Vector2f nw = blockPos;
    sf::Vector2f ne = blockPos + sf::Vector2f(blockSize.x, 0);
    sf::Vector2f sw = blockPos + sf::Vector2f(0, blockSize.y);
    sf::Vector2f se = blockPos + blockSize;
    sf::Vector2f corner = sf::Vector2f(-1, -1);
    for (auto& corn : { nw, ne, sw, se })
        if (ball.isCloseTo(corn))
            corner = corn;
    if (corner == sf::Vector2f(-1, -1))
        return false;
    sf::Vector2f axis = corner - midPoint;
    if (Dot(velocity, axis) > 0) {
        sf::Vector2f velProjAxis = Dot(velocity, axis) / Dot(axis, axis) * axis;
        sf::Vector2f velProjConj = velocity - velProjAxis;
        sf::Vector2f newVelocity = velProjConj - velProjAxis;
        ball.setVelocity(newVelocity);
    }
    else {
        float push = sqrt(radius * radius - axis.y * axis.y) - abs(axis.x);
        ball.setPosition(pos + sf::Vector2f(velocity.x > 0 ? push : -push, 0));
       
        if (axis.y * velocity.y > 0)
            ball.setVelocity(sf::Vector2f(velocity.x, -velocity.y));
    }
    return true;
}

// Конструктор разрушаемого блока
SoftBlock::SoftBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
    const int health, const int hitScore, const sf::Color& color) :
    Block(parent, pos, size, color), _health(health), _healthDefault(health), _hitScore(hitScore) {}

bool SoftBlock::isDead() const { // проверка живучести
    return _health < 1;
}

void SoftBlock::damage() {// Получение урона
    if (_health < 1)
        return;
    _health--;
    const sf::Color fillColor = _shape.getFillColor();
    const sf::Color outlineColor = _shape.getOutlineColor();
    const sf::Uint8 newAlpha = static_cast<sf::Uint8>(255 * (_health / static_cast<float>(_healthDefault)));
    _shape.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, newAlpha));
    _shape.setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, newAlpha));
}

std::function<void(void)> SoftBlock::collideWith(DefenderBall& ball) const {
    if (isDead())
        return [](void) -> void { return; };
    sf::Vector2f myPos = getPosition();
    sf::Vector2f otherPos = ball.getPosition();
    return [=, &ball](void) -> void {
        if (_collideWithBall(otherPos, myPos, ball))
            _parent.increaseScore(_hitScore);
        };
}

DeathActionBlock::DeathActionBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
    const int health, const int hitScore, const sf::Color& color) :
    SoftBlock(parent, pos, size, health, hitScore, color) {}

std::function<void(void)> DeathActionBlock::collideWith(DefenderBall& ball) const {
    if (isDead())
        return [](void) -> void { return; };
    const unsigned health = _health;
    const sf::Vector2f pos = ball.getPosition();
    const sf::Vector2f racketPos = _shape.getPosition();
    return [=, &ball](void) -> void {
        if (_collideWithBall(pos, racketPos, ball)) {
            _parent.increaseScore(_hitScore);
            if (health == 1)
                _deathAction(ball);
        }
        };
}

BoostBlock::BoostBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size,
    const int health, const float multiplier, const float durationMs,
    const int hitScore, const sf::Color& color) :
    DeathActionBlock(parent, pos, size, health, hitScore, color), _multiplier(multiplier), _durationMs(durationMs) {}

void BoostBlock::_deathAction(DefenderBall& ball) const {
    ball.boostUp(_multiplier, _durationMs);
}

BonusBlock::BonusBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, const int health, const int hitScore, const sf::Color& color) :
    DeathActionBlock(parent, pos, size, health, hitScore, color) {}

void BonusBlock::_deathAction(DefenderBall& ball) const {
    sf::Vector2f spawnPos = getPosition() + getSize() / 2.0f;
    _parent.spawnRandomBonus(spawnPos);
}

MovingBlock::MovingBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, const float speed,
    const int health, const int hitScore, const sf::Color& color) :
    SoftBlock(parent, pos, size, health, hitScore, color), _speed(speed) {}

void MovingBlock::setSpeed(const float speed) {
    _speed = speed;
}

float MovingBlock::getSpeed() const {
    return _speed;
}

void MovingBlock::setPosition(const sf::Vector2f& pos) {
    _shape.setPosition(pos);
}

void MovingBlock::draw() {
    _update();
    _parent.getWindow()->draw(_shape);
}

void MovingBlock::_move() {
    float timeDeltaMs = _parent.getTimeMsSinceLastFrame();
    _shape.setPosition(getPosition() + timeDeltaMs * sf::Vector2f(_speed, 0));
}

void MovingBlock::_update() {
    _move();
    _handleWindowCollision();
}

void MovingBlock::_handleWindowCollision() {
    sf::Vector2f pos = getPosition();
    float width = getSize().x;
    unsigned winWidth = _parent.getWindow()->getSize().x;
    if (pos.x < 0) {
        _shape.setPosition(sf::Vector2f(0, pos.y));
        _speed = -_speed;
    }
    if (pos.x + width > winWidth) {
        _shape.setPosition(sf::Vector2f(winWidth - width, pos.y));
        _speed = -_speed;
    }
}