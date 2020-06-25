//
// Created by Koko on 04.06.2020.
//

#include "EntityManager.h"


namespace core {
    EntityManager *EntityManager::s_instance = nullptr;

    EntityManager::EntityManager() {
        if (!s_instance) {
            s_instance = this;
        }
        else {
            throw std::runtime_error("EntityManager is a singleton class");
        }
    }

    void EntityManager::update(float time) {
        for (auto &entity: m_entities) {
            entity->update(time);
        }
    }

    void EntityManager::draw() const {
        for (auto &entity: m_entities) {
            entity->draw();
        }
    }

    void EntityManager::add(Entity *entity) {
        m_entities.emplace_back(entity);
    }
} // namespace core