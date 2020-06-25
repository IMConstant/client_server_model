//
// Created by Koko on 04.06.2020.
//

#ifndef UNTITLED1_TYPEPRIMITIVES_H
#define UNTITLED1_TYPEPRIMITIVES_H

#include <gl/glew.h>
#include <glm/glm.hpp>

namespace core {

    using vec2 = glm::vec2;
    using vec3 = glm::vec3;

    namespace gl {

        enum PrimitiveType {
            Points        = GL_POINTS,
            Lines         = GL_LINES,
            LineLoop      = GL_LINE_LOOP,
            LineStrip     = GL_LINE_STRIP,
            Polygon       = GL_POLYGON,
            Quads         = GL_QUADS,
            QuadStrip     = GL_QUAD_STRIP,
            Triangles	  = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan   = GL_TRIANGLE_FAN
        };

    } // namespace gl

} // namespace core

#endif //UNTITLED1_TYPEPRIMITIVES_H
