//
// Created by Koko on 05.06.2020.
//

#ifndef UNTITLED1_USERCONTROLCOMPONENT_H
#define UNTITLED1_USERCONTROLCOMPONENT_H

#include "../client/src/Application.h"

#include <chrono>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/compatibility.hpp>

#include "../core/objects/components/Component.h"
#include "../client/src/Input.h"
#include "../client/src/KeyboardButtons.h"

#include "../core/managers/EntityManager.h"


struct UserControlComponent : public core::Component {
    COMPONENT

private:
    glm::vec2 direction;
    glm::vec2 look_direction;
    float speed;
    float maxSpeed;
    float angle;

    float boostActivationTime;
    float boostActiveTime;
    bool  boostActivated;

public:
    explicit UserControlComponent(glm::vec2 _look_direction);
    virtual ~UserControlComponent() {}

    void setDirection(glm::vec2 _direction) {
        direction = _direction;
    }

    float getAngle() const {
        return angle;
    }

    void init() override;
};


#endif //UNTITLED1_USERCONTROLCOMPONENT_H
