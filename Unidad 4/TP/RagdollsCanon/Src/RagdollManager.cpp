#include "RagdollManager.h"

RagdollManager::RagdollManager(b2World& world) : world_(world) {}

void RagdollManager::createRagdoll(const b2Vec2& position, const b2Vec2& impulse) {
    std::unique_ptr<Ragdoll> ragdoll = std::make_unique<Ragdoll>();
    ragdoll->create(world_, position.x, position.y);
    ragdoll->applyImpulse(impulse);
    ragdolls_.push_back(std::move(ragdoll));
}

void RagdollManager::render(sf::RenderWindow& window) {
    for (const auto& ragdoll : ragdolls_) {
        ragdoll->draw(window);
    }
}