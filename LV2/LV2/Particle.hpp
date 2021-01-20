#pragma once
#include <vector>
#include <GLM/glm.hpp>

struct Particle {
    glm::vec3 Position, Velocity, Force;
    double Life, MaxLife;

    Particle();

    Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 randVel, double life);
};

unsigned int FindDeadParticle(std::vector<Particle>& particles);

float random(float min, float max);

void RespawnParticle(Particle& particle, glm::vec3 pos, glm::vec3 vel, double life);