//
// Created by Koko on 04.06.2020.
//

#include "Entity.h"


namespace core {

    Entity::Entity(core::Entity *parent) : m_parent(parent), m_componentArray(ComponentArray()) {}

    void Entity::update(float time) {
        for (auto &component: m_components) {
            component->update(time);
        }
    }

    void Entity::draw() const {
        for (auto &component: m_components) {
            component->draw();
        }
    }

} // namespace core