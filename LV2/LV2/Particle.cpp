#pragma once
#include "Particle.hpp"

Particle::Particle() : Position(0.0f), Velocity(0.0f), Force(0.0f), Life(0.0f), MaxLife(0.0f) {}

Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 randVel, double life) : Position(pos), Velocity(vel + randVel), Force(randVel* glm::vec3(-1.15f)), Life(life), MaxLife(life) {}

unsigned int FindDeadParticle(std::vector<Particle>& particles)
{
    static unsigned int nextParticleToCheck = particles.size();

    for (unsigned int i = nextParticleToCheck; i < particles.size(); i++) {
        if (particles[i].Life <= 0.0f) {
            nextParticleToCheck = i + 1;
            return i;
        }
    }

    for (unsigned int i = 0; i < nextParticleToCheck; i++) {
        if (particles[i].Life <= 0.0f) {
            nextParticleToCheck = i + 1;
            return i;
        }
    }

    return particles.size();
}

float random(float min, float max) {
    float r = float(rand());
    r /= RAND_MAX;
    return r * max + (1 - r) * min;
}

float POS = 0.0f;
float VEL = 0.4f;

void RespawnParticle(Particle& particle, glm::vec3 pos, glm::vec3 vel, double life) {
    pos += glm::vec3(random(-POS, POS), random(-POS, POS), random(-POS, POS));
    glm::vec3 randVel(random(-VEL, VEL), random(-VEL, VEL), random(-VEL, VEL));
    particle = Particle(pos, vel, randVel, life);
}