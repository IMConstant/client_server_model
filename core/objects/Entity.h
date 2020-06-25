//
// Created by Koko on 04.06.2020.
//

#ifndef UNTITLED1_ENTITY_H
#define UNTITLED1_ENTITY_H

#include <vector>
#include <memory>
#include <bitset>

#include "components/Component.h"


namespace core {
    class Component;

    constexpr std::size_t maxComponents{32};

    using ComponentArray = std::array<Component *, maxComponents>;
    using ComponentBitset = std::bitset<maxComponents>;

    template<typename T> unsigned int getComponentId();

    class Entity {
        Entity *m_parent;
        std::vector<std::unique_ptr<Component>> m_components;

        ComponentArray m_componentArray;
        ComponentBitset m_componentBitset;

    public:
        Entity(Entity *parent = nullptr);

        virtual void update(float time);
        virtual void draw() const;

        template <typename T> bool hasComponent();
        template <typename T, typename ... Args> T &addComponent(Args &&... args);
        template <typename T> T &getComponent();
    };



    template <typename T>
    bool Entity::hasComponent() {
        return m_componentArray[getComponentId<T>()];
    }

    template <typename T, typename ... Args>
    T &Entity::addComponent(Args &&... args) {
        assert(!hasComponent<T>());

        auto *component = new T(std::forward<Args>(args) ...);
        std::unique_ptr<Component> uPtr{component};
        m_components.emplace_back(std::move(uPtr));

        m_componentArray[getComponentId<T>()] = component;
        m_componentBitset[getComponentId<T>()] = true;

        component->entity = this;
        component->init();

        return *component;
    }

    template <typename T>
    T &Entity::getComponent() {
        assert(hasComponent<T>());

        return *static_cast<T *>(m_componentArray[getComponentId<T>()]);
    }

} // namespace core

#endif //UNTITLED1_ENTITY_H
