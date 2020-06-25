//
// Created by Koko on 14.06.2020.
//

#ifndef UNTITLED1_SHAPECOMPONENT_H
#define UNTITLED1_SHAPECOMPONENT_H

#include "Component.h"


namespace core {

    struct ShapeComponent : public Component {
        COMPONENT

        ShapeComponent(gl::PrimitiveType type = gl::Polygon);

    private:
        std::vector<gl::Vertex> m_vertices;
        gl::PrimitiveType m_type;

    public:
        void init() override;

        void clear();
        void addVertex(gl::Vertex vertex);

    };

} // namespace core


#endif //UNTITLED1_SHAPECOMPONENT_H
