//
// Created by Koko on 05.06.2020.
//

#include "UserControlComponent.h"


UserControlComponent::UserControlComponent(glm::vec2 _look_direction) : look_direction(_look_direction) {}

void UserControlComponent::init() {
    look_direction = { 1.0f, 0.0f };
    speed = 0.0f;
    maxSpeed = 430.0f;
    angle = 0.0f;
    boostActivationTime = 2.2f;
    boostActiveTime = 0.0f;
    boostActivated = false;
}

void UserControlComponent::update(float time) {
    auto &cPosition = entity->getComponent<core::PositionComponent>();
    auto &particleSystem = Application::getInstance()->getParticleSystem();

    static glm::vec2 last_direction{};

    bool boost = Input::isKeyPressed(KEY_SPACE);

    if (boost && !boostActivated) {
        boostActiveTime += time;

        if (boostActiveTime >= boostActivationTime) {
            boostActivated = true;
        }
        else {
            float p = boostActiveTime / boostActivationTime;

            for (int i = 0; i < 1 + static_cast<int>(5 * p); i++) {
                int pc = std::rand() % 10;

                ParticleProps props{};
                props.lifeTime = (pc == 0 ? ((std::rand() / (float) RAND_MAX) * 4.5f) : 2.0f);
                props.position.x = cPosition.position.x;
                props.position.y = cPosition.position.y;
                props.begin_color = ImVec4(1.0f, 1.0f - p, 0.0f, 1.0f - p);
                props.end_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f - p);
                props.sizeBegin = 3 + std::rand() % 5;
                props.pol_count = 3 + std::rand() % 3;

                static const double pi = std::acos(-1);

                float angle = -50.0f + 100.0f * (std::rand() / (float) RAND_MAX);
                angle *= (1.0f - p);
                float r = (3.0f + (pc == 0 ? 1.3f : 2.0f)) * (std::rand() / (float) RAND_MAX);
                glm::vec2 particle_direction = r *  glm::rotate(-look_direction, glm::radians(angle));

                props.direction = particle_direction;

                particleSystem.emit(props);
            }
        }
    }
    else if (!boost) {
        boostActiveTime = 0.0f;
        boostActivated = false;
    }

    bool windowFocused = !ImGui::IsAnyWindowFocused();

    float max_s = maxSpeed * (1 + (boostActivated ? 5 : 0));

    if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

    }

    if (windowFocused && ((Input::isKeyPressed(KEY_W) && !boost) || boostActivated)) {
        direction += (boostActivated ? 10.2f : 5.1f) * look_direction;

        speed += std::min(max_s, 0.05f * max_s * (1.0f - (speed / max_s)));

        for (int i = 0; i < (boostActivated ? 150 : 5); i++) {
            int pc = std::rand() % 10;

            ParticleProps props{};
            props.lifeTime = (pc == 0 ? ((std::rand() / (float) RAND_MAX) * 4.5f) : 2.0f);
            props.position.x = cPosition.position.x;
            props.position.y = cPosition.position.y;

            if (boostActivated) {
                props.begin_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else {
                props.begin_color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
            }

            props.end_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
            props.sizeBegin = 3 + std::rand() % 5;
            props.pol_count = 3 + std::rand() % 3;

            static const double pi = std::acos(-1);

            float angle = -50.0f + 100.0f * (std::rand() / (float) RAND_MAX);
            float r = (50.0f + (pc == 0 ? 100.3f : 400.0f)) * (std::rand() / (float) RAND_MAX);
            glm::vec2 particle_direction = r *  glm::rotate(-look_direction, glm::radians(angle));

            props.direction = particle_direction;

            particleSystem.emit(props, [ed = -look_direction, ep = props.position](Particle &particle, float time) {
                glm::vec2 v1 = (ed.x == 0.0f ? glm::vec2(1.0f, 0.0f) : glm::vec2(-ed.y / ed.x, 1.0f));
                v1 /= glm::length(v1);
                glm::vec2 offset = glm::distance(ep + v1, particle.position) < glm::distance(ep - v1, particle.position) ? -v1 : v1;

                particle.direction += 7.5f * offset;


                particle.position += particle.direction * time;
            });
        }
    }

    //speed = direction.length();
    direction *= 0.98f;

    if (windowFocused && Input::isKeyPressed(KEY_D)) {
        angle -= 3.0f;
        look_direction = glm::rotate(look_direction, -glm::radians(3.0f));
        entity->getComponent<core::Transformable>().rotate(-3.0f);
    }

    if (windowFocused && Input::isKeyPressed(KEY_A)) {
        angle += 3.0f;
        look_direction = glm::rotate(look_direction, glm::radians(3.0f));
        entity->getComponent<core::Transformable>().rotate(3.0f);
    }

    cPosition.position += direction * time;
}

void UserControlComponent::draw() const { }