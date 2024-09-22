#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

/**
 * @brief A class representing a particle in a particle system.
 *
 * Each particle has properties such as position, velocity, color, life span, and size.
 * It can be updated based on its velocity and the elapsed time, and it can check for collisions
 * with defined boundaries.
 */
class Particle {
public:
    glm::vec3 position; ///< The current position of the particle.
    glm::vec3 velocity; ///< The current velocity of the particle.
    glm::vec4 color; ///< The color of the particle, including alpha for transparency.
    float life; ///< The remaining life of the particle. If life is <= 0, the particle is considered dead.
    float size; ///< The size of the particle.
    GLfloat wallXMin, wallXMax; ///< The minimum and maximum X boundaries for collision detection.
    GLfloat wallYMin, wallYMax; ///< The minimum and maximum Y boundaries for collision detection.
    GLfloat wallHeight; ///< The height of the wall or ceiling for collision detection.

    /**
     * @brief Constructor to initialize a particle with given parameters.
     *
     * @param pos The initial position of the particle.
     * @param vel The initial velocity of the particle.
     * @param col The color of the particle.
     * @param lifespan The total lifespan of the particle.
     * @param sz The size of the particle.
     * @param xMin The minimum X boundary for collision detection.
     * @param xMax The maximum X boundary for collision detection.
     * @param yMin The minimum Y boundary for collision detection.
     * @param yMax The maximum Y boundary for collision detection.
     * @param height The height of the wall or ceiling for collision detection.
     */
    Particle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec4& col, float lifespan, float sz,
        GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat height)
        : position(pos), velocity(vel), color(col), life(lifespan), size(sz),
        wallXMin(xMin), wallXMax(xMax), wallYMin(yMin), wallYMax(yMax), wallHeight(height) {}

    /**
     * @brief Update the particle's state based on elapsed time.
     *
     * This method applies gravity to the particle's velocity, updates its position, and reduces its life.
     * It also checks for collisions with the defined boundaries and deactivates the particle if a collision is detected.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void update(float deltaTime) {
        // Apply reduced gravity to the particle's velocity
        velocity.y -= 1.0f * deltaTime; // Gravity force for slower falling

        // Update the particle's position based on its velocity
        position += velocity * deltaTime;

        // Decrease the particle's life by the elapsed time
        life -= deltaTime;

        // Check for collisions with the boundaries
        if (position.x - size < wallXMin || position.x + size > wallXMax ||
            position.y - size < 0 || position.y + size > wallHeight ||
            position.z - size < wallYMin || position.z + size > wallYMax) {
            life = 0; // Deactivate the particle
        }
    }

    /**
     * @brief Check if the particle is still alive.
     *
     * @return True if the particle's life is greater than 0, otherwise false.
     */
    bool isAlive() const {
        return life > 0.0f;
    }
};

#endif // PARTICLE_H
