//
// Created by Koko on 14.06.2020.
//

#include "ShapeComponent.h"


namespace core {

    ShapeComponent::ShapeComponent(core::gl::PrimitiveType type) : m_type(type) {}

    void ShapeComponent::update(float time) {

    }

    void ShapeComponent::draw() const {
        auto &cPosition = entity->getComponent<PositionComponent>();
        auto &transform = entity->getComponent<Transformable>();

        glPushMatrix();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glm::mat4 view_matrix = transform.setTranslation(cPosition.position) * transform.getRotation();
        glLoadMatrixf(reinterpret_cast<float *>(&view_matrix));

        glBegin(GL_TRIANGLES);

        for (int i = 0; i < 3; i++) {
            glColor4f(1.0f, 0.0f, 0.0f, i == 0);

            glVertex2f(30 * std::cos(i * (2 * math::pi / 3.0f)),
                       10 * std::sin(i * (2 * math::pi / 3.0f)));
        }

        glEnd();

        glPopMatrix();
    }

    void ShapeComponent::init() {
        assert(!entity->hasComponent<PositionComponent>());
    }

    void ShapeComponent::clear() {
        m_vertices.clear();
    }

    void ShapeComponent::addVertex(gl::Vertex vertex) {
        m_vertices.push_back(vertex);
    }

} // namespace core