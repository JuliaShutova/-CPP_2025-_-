#define _USE_MATH_DEFINES

#include "Racket.h"
#include "Game.h"
#include <cmath>

constexpr float ANGLE_MAX = 1.3f; //������

Racket::Racket(Game& parent, const float y, const float speed, const sf::Vector2f& size, const sf::Color& color) :
    _parent(parent), _speedDefault(speed), _speed(0) {
    _shape.setFillColor(color);
    _shape.setSize(size);
    _shape.setPosition(sf::Vector2f(0, y));
}

void Racket::handleEvent(const sf::Event& event) { // ������� �������
    switch (event.type) {
    case sf::Event::KeyPressed:
        switch (event.key.code) {
        case sf::Keyboard::Left:
            if (_speed == 0)
                _speed = -_speedDefault;
            break;
        case sf::Keyboard::Right:
            if (_speed == 0)
                _speed = _speedDefault;
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch (event.key.code) {
        case sf::Keyboard::Left:
            if (_speed < 0)
                _speed = 0;
            break;
        case sf::Keyboard::Right:
            if (_speed > 0)
                _speed = 0;
            break;
        }
        break;
    }
}

sf::Vector2f Racket::getPosition() const {
    return _shape.getPosition();
}

sf::Vector2f Racket::getSize() const {
    return _shape.getSize();
}


std::function<void(void)> Racket::collideWith(DefenderBlock& block) const { //�����
    return [](void) -> void { return; };
}

std::function<void(void)> Racket::collideWith(DefenderBonus& bonus) const { // ������
    sf::FloatRect self = sf::FloatRect(getPosition(), getSize());
    sf::FloatRect bonusRect = sf::FloatRect(bonus.getPosition(), bonus.getSize());
    if (!self.intersects(bonusRect) || bonus.isDead())
        return [](void) -> void { return; };
    return [&bonus](void) -> void { bonus.invokeBonusAction(); };
}

std::function<void(void)> Racket::collideWith(DefenderBall& ball) const { //���
    const sf::Vector2f pos = ball.getPosition();
    const sf::Vector2f racketPos = _shape.getPosition();
    return [=, &ball](void) -> void {
        if (_parent.isStickyBallEnabled()) {
            // ���������, �������� �� ����� �������
            sf::Vector2f racketSize = _shape.getSize();
            float ballRadius = ball.getRadius();
            sf::FloatRect top = sf::FloatRect(
                racketPos - sf::Vector2f(0, ballRadius),
                sf::Vector2f(racketSize.x, ballRadius)
            );
            sf::Vector2f ballMidPoint = pos + sf::Vector2f(ballRadius, ballRadius);

            if (top.contains(ballMidPoint)) {
                // ��������� ����� � �������
                ball.setPosition(sf::Vector2f(
                    ballMidPoint.x - ballRadius,
                    racketPos.y - ballRadius * 2
                ));
                ball.setVelocity(sf::Vector2f(0, 0)); // ������������� ���
            }
        }
        else {
            _collideWithBall(pos, racketPos, ball);
        }
        };
}

bool Racket::_collideWithBall(const sf::Vector2f& ballBackupPos, const sf::Vector2f& selfBackupPos, DefenderBall& ball) const {
    // ������� ������������
    sf::Vector2f pos = ballBackupPos;
    sf::Vector2f racketPos = selfBackupPos;
    sf::Vector2f racketSize = _shape.getSize();
    sf::Vector2f velocity = ball.getVelocity();
    float radius = ball.getRadius();
    sf::Vector2f midPoint = pos + sf::Vector2f(radius, radius);
   
    sf::FloatRect top = sf::FloatRect(
        racketPos - sf::Vector2f(0, radius),
        sf::Vector2f(racketSize.x, radius)
    );
    sf::FloatRect bot = sf::FloatRect(
        racketPos + sf::Vector2f(0, racketSize.y),
        sf::Vector2f(racketSize.x, radius)
    );
    sf::FloatRect left = sf::FloatRect(
        racketPos - sf::Vector2f(radius, 0),
        sf::Vector2f(radius, racketSize.y)
    );
    sf::FloatRect right = sf::FloatRect(
        racketPos + sf::Vector2f(racketSize.x, 0),
        sf::Vector2f(radius, racketSize.y)
    );
    

    if (top.contains(midPoint)) {
        float velocityNorm = sqrt(DistSqr(velocity, sf::Vector2f(0, 0)));
        float angle = (pos.x + radius - (racketPos.x + racketSize.x / 2.0f)) / (racketSize.x / 2.0f) * ANGLE_MAX;
        sf::Vector2f newVelocity = {
            velocityNorm * sin(angle),
            -velocityNorm * cos(angle)
        };
        ball.setPosition(sf::Vector2f(pos.x, top.top - radius));
        ball.setVelocity(newVelocity);
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
    
    sf::Vector2f nw = racketPos;
    sf::Vector2f ne = racketPos + sf::Vector2f(racketSize.x, 0);
    sf::Vector2f sw = racketPos + sf::Vector2f(0, racketSize.y);
    sf::Vector2f se = racketPos + racketSize;
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

void Racket::_move() { // �����������
    float timeDeltaMs = _parent.getTimeMsSinceLastFrame();
    sf::Vector2f velocity = sf::Vector2f(_speed, 0);
    _shape.move(timeDeltaMs * velocity);
}

void Racket::_update() {
    _move();
    _handleWindowCollision();
}

void Racket::_handleWindowCollision() {
    sf::Vector2f pos = _shape.getPosition();
    float width = _shape.getSize().x;
    unsigned winWidth = _parent.getWindow()->getSize().x;
    if (pos.x < 0)
        _shape.setPosition(sf::Vector2f(0, pos.y));
    if (pos.x + width > winWidth)
        _shape.setPosition(sf::Vector2f(winWidth - width, pos.y));
}

void Racket::setSize(const sf::Vector2f& size) {
    _shape.setSize(size);
}


void Racket::draw() {
    _update();
    _parent.getWindow()->draw(_shape);
}

