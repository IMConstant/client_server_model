//
// Created by Koko on 01.06.2020.
//

#include "ParticleSystem.h"

#include <utility>


const ParticleHandler ParticleSystem::HandlerDefault = [](Particle &particle, float time) noexcept {
    particle.position += particle.direction * time;
};


ParticleSystem::ParticleSystem() : maxParticles(30000) {
    pool = std::vector<Particle>(maxParticles, Particle());
}

void ParticleSystem::update(float time) {
    activeCount = 0;

    for (auto &particle: pool) {
        if (!particle.active) {
            continue;
        }

        particle.lifeRemaining -= time;

        if (particle.lifeRemaining <= 0) {
            particle.active = false;
        }

        if (particle.active) {
            particle.on_update(particle, time);

            activeCount++;
        }
    }
}

void ParticleSystem::render(float time) {
    static const double pi = std::acos(-1);

    for (auto &particle: pool) {
        if (particle.active) {
            glBegin(GL_POLYGON);
            float p = particle.lifeRemaining / particle.lifeTime; // 1 --> 0
            const ImVec4 &begin_color = particle.begin_color;
            const ImVec4 &end_color = particle.end_color;

            glColor4f(begin_color.x + (1.0f - p) * (end_color.x - begin_color.x),
                    begin_color.y + (1.0f - p) * (end_color.y - begin_color.y),
                     begin_color.z + (1.0f - p) * (end_color.z - begin_color.z),
                    0.6f * p);

            int range = particle.pol_count;

            for (int i = 0; i < range; i++) {
                glVertex2f(particle.position.x + particle.sizeBegin * p * std::cos(i * (2.0f * pi / (float)range)),
                        particle.position.y + particle.sizeBegin * p *  std::sin(i * (2.0f * pi / (float)range)));
            }

            glEnd();
        }
    }
}

void ParticleSystem::emit(ParticleProps props, ParticleHandler handler) {
    Particle &particle = pool[poolIndex];

    particle.active = true;
    particle.position = props.position;
    particle.direction = props.direction;
    particle.lifeTime = props.lifeTime;
    particle.lifeRemaining = particle.lifeTime;
    particle.begin_color = props.begin_color;
    particle.end_color = props.end_color;
    particle.sizeBegin = props.sizeBegin;
    particle.pol_count = props.pol_count;
    particle.on_update = std::move(handler);

    poolIndex = ++poolIndex % maxParticles;
}