//
// Created by Koko on 04.06.2020.
//

#ifndef UNTITLED1_COMPONENT_H
#define UNTITLED1_COMPONENT_H

#include "../../TypePrimitives.h"
#include "../../Vertex.h"

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "../Entity.h"
#include "../../math/math.h"


#define COMPONENT void update(float time) override;\
    void draw() const override;\

namespace core {
    class Entity;
    struct Component;

    using ComponentId = std::size_t;

    ComponentId getUniqueComponentId();
    template<typename T> unsigned int getComponentId();

    template<typename T>
    unsigned int getComponentId() {
        static_assert(std::is_base_of<Component, T>::value, "T must be inherited from Component");

        static ComponentId componentId{getUniqueComponentId()};
        return componentId;
    }

    struct Component {
        Entity *entity;

        virtual void init() {}

        virtual void update(float time) = 0;
        virtual void draw() const = 0;

        virtual ~Component() {}
    };


    struct PositionComponent : public Component {
        COMPONENT

        vec2 position;

        PositionComponent() = default;
        PositionComponent(vec2 _position);

        virtual ~PositionComponent() {}
    };


    struct Transformable : public Component {
        COMPONENT

    private:
        glm::mat4 m_translation;
        glm::mat4 m_rotation;
        glm::mat4 m_scale;

    public:
        virtual ~Transformable() {}

        void init() override;

        const glm::mat4 &rotate(float angle);
        const glm::mat4 &translate(glm::vec2 dir);
        const glm::mat4 &translate(float x, float y);
        const glm::mat4 &scale(glm::vec2 val);
        const glm::mat4 &scale(float x, float y);

        const glm::mat4 &setTranslation(glm::vec2 dir);

        const glm::mat4 &setRotation(float angle);

        const glm::mat4 &getTranslation() const {
            return m_translation;
        }

        const glm::mat4 &getRotation() const {
            return m_rotation;
        }
    };
}


#endif //UNTITLED1_COMPONENT_H
