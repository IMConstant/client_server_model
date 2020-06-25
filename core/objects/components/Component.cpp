//
// Created by Koko on 04.06.2020.
//

#include "Component.h"


namespace core {

    ComponentId getUniqueComponentId() {
        static ComponentId id{0};

        return id++;
    }


    PositionComponent::PositionComponent(core::vec2 _position) : position(_position) {}

    void PositionComponent::update(float time) { }
    void PositionComponent::draw() const { }

    void Transformable::init() {
        m_translation = glm::mat4(1.0f);
        m_rotation = glm::mat4(1.0f);
        m_scale = glm::mat4(1.0f);
    }

    void Transformable::update(float time) {

    }

    void Transformable::draw() const {

    }

    const glm::mat4 &Transformable::rotate(float angle) {
        m_rotation = glm::rotate(m_rotation, glm::radians(angle), { 0, 0, 1 });

        return m_rotation;
    }

    const glm::mat4 &Transformable::translate(glm::vec2 dir) {
        return translate(dir.x, dir.y);
    }

    const glm::mat4 &Transformable::translate(float x, float y) {
        m_translation = glm::translate(m_translation, { x, y, 0 });

        return m_translation;
    }

    const glm::mat4 &Transformable::scale(glm::vec2 val) {
        return scale(val.x, val.y);
    }

    const glm::mat4 &Transformable::scale(float x, float y) {
        m_scale = glm::scale(m_scale, { x, y, 0 });
    }

    const glm::mat4 &Transformable::setTranslation(glm::vec2 dir) {
        m_translation = glm::mat4(1.0f);

        return translate(dir);
    }

    const glm::mat4 &Transformable::setRotation(float angle) {
        m_rotation = glm::mat4(1.0f);

        return rotate(angle);
    }

} // namespace core