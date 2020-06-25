//
// Created by Koko on 13.06.2020.
//

#ifndef UNTITLED1_VERTEX_H
#define UNTITLED1_VERTEX_H

#include <glm/glm.hpp>


namespace core {

    namespace gl {

        struct Color {
            Color(float _r, float _g, float _b, float _a) :
                    m_data(_r, _g, _b, _a) {}

            float r() const {
                return m_data.x;
            }

            float g() const {
                return m_data.y;
            }

            float b() const {
                return m_data.z;
            }

            float a() const {
                return m_data.w;
            }

        private:
            glm::vec4 m_data;
        };


        struct Vertex {
            Vertex(glm::vec2 _vertex, Color _color, glm::vec2 _normal = glm::vec2(1, 0)) :
                vertex(_vertex), color(_color), normal(_normal) {}

            glm::vec2 vertex;
            Color color;
            glm::vec2 normal;
        };

    } // namespace gl

} // namespace core


#endif //UNTITLED1_VERTEX_H
