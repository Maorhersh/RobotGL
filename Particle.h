#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float life;
    float size;
    GLfloat wallXMin, wallXMax, wallYMin, wallYMax, wallHeight;

    Particle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec4& col, float lifespan, float sz, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat height)
        : position(pos), velocity(vel), color(col), life(lifespan), size(sz), wallXMin(xMin), wallXMax(xMax), wallYMin(yMin), wallYMax(yMax), wallHeight(height) {}

    void update(float deltaTime) {
        // Apply reduced gravity
        velocity.y -= 1.0f * deltaTime; // Reduced gravity force for slower falling

        position += velocity * deltaTime;
        life -= deltaTime;

        // Check for collisions with walls and ceiling
        if (position.x - size < wallXMin || position.x + size > wallXMax ||
            position.y - size < 0 || position.y + size > wallHeight ||
            position.z - size < wallYMin || position.z + size > wallYMax) {
            life = 0; // Pop the bubble
        }
    }

    bool isAlive() const {
        return life > 0.0f;
    }
};

#endif // PARTICLE_H
