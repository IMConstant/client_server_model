//
// Created by Koko on 01.06.2020.
//

#ifndef UNTITLED1_PARTICLESYSTEM_H
#define UNTITLED1_PARTICLESYSTEM_H

#include <GL/glew.h>

#include "Event.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <functional>

#include <imgui.h>
#include <glm/glm.hpp>


struct Particle;

using ParticleHandler = std::function<void(Particle &, float)>;

struct ParticleProps {
    glm::vec2 position;
    glm::vec2 direction;
    float lifeTime;
    float lifeRemaining;

    float emitX;
    int pol_count;

    ImVec4 begin_color;
    ImVec4 end_color;
    float sizeBegin;
    float sizeEnd;
};


struct Particle {
    glm::vec2 position;
    glm::vec2 direction;
    bool active = false;
    float lifeTime;
    float lifeRemaining;

    float emitX;
    int pol_count;

    ImVec4 begin_color;
    ImVec4 end_color;
    float sizeBegin;
    float sizeEnd;

    ParticleHandler on_update;

    Particle() = default;
};

class ParticleSystem {
    std::vector<Particle> pool;
    unsigned int poolIndex = 0;
    unsigned int maxParticles = 1000;
    int activeCount;

public:
    ParticleSystem();

    int getParticlesCount() const {
        return activeCount;
    }

    void emit(ParticleProps props, ParticleHandler handler = HandlerDefault);
    void update(float time);
    void render(float time);

    static const ParticleHandler HandlerDefault;
};


#endif //UNTITLED1_PARTICLESYSTEM_H
