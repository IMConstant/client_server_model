//
// Created by Koko on 15.06.2020.
//

#include "DirectionComponent.h"


namespace core {

    void DirectionComponent::init() {
        cPosition = entity->getComponent<PositionComponent>();
    }

    void DirectionComponent::update(float time) {
        cPosition.position += direction;
    }

    void DirectionComponent::draw() const {

    }

} // namespace core