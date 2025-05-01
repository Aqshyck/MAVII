#include "Cannon.h"
#include <cmath>

Cannon::Cannon(b2World& world)
    : world_(world),
    angle_(0.0f),
    power_(0.0f),
    maxPower_(50.0f),
    position_(20.0f, 80.0f) {
    shape_.setSize(sf::Vector2f(30.0f, 10.0f));
    shape_.setFillColor(sf::Color::Red);
    shape_.setOrigin(0.0f, 5.0f);
    shape_.setPosition(position_);
}

void Cannon::handleInput(RagdollManager& ragdollManager) {
    
    b2Vec2 cannonPos(position_.x / 50.0f, (900.0f - position_.y) / 50.0f);
    ragdollManager.createRagdoll(cannonPos, b2Vec2(power_ * std::cos(angle_), power_ * std::sin(angle_)));
    
}

void Cannon::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float dx = mousePos.x - position_.x;
    float dy = position_.y - mousePos.y;
    angle_ = std::atan2(dy, dx);
    float distance = std::sqrt(dx * dx + dy * dy);
    power_ = std::min(distance / 1600.0f * maxPower_, maxPower_);
    shape_.setRotation(-angle_ * 180.0f / b2_pi);
}

void Cannon::render(sf::RenderWindow& window) {
    window.draw(shape_);
}