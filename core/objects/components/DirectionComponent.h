//
// Created by Koko on 15.06.2020.
//

#ifndef UNTITLED1_DIRECTIONCOMPONENT_H
#define UNTITLED1_DIRECTIONCOMPONENT_H

#include "Component.h"


namespace core {

    struct DirectionComponent : public Component {
        COMPONENT

        void init() override;

        glm::vec2 direction;
        PositionComponent &cPosition;
    };

} // namespace core


#endif //UNTITLED1_DIRECTIONCOMPONENT_H
