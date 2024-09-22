#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <GL/glu.h> // Include GLU for drawing spheres

/**
 * @brief A class representing a particle system.
 *
 * This class manages a collection of particles, allowing them to be updated and drawn.
 * Each particle is represented by an instance of the Particle class.
 */
class ParticleSystem {
public:
    std::vector<Particle> particles; ///< List of particles in the system

    /**
     * @brief Add a particle to the system.
     *
     * @param particle The particle to be added.
     */
    void addParticle(const Particle& particle) {
        particles.push_back(particle);
    }

    /**
     * @brief Update the particle system.
     *
     * This method updates each particle's state based on the elapsed time (deltaTime)
     * and removes particles that are no longer alive.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void update(float deltaTime) {
        // Update each particle's state
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }
        // Remove dead particles
        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) { return !p.isAlive(); }), particles.end());
    }

    /**
     * @brief Draw the particle system.
     *
     * This method renders each particle as a sphere with a bubble-like appearance.
     * It uses blending and lighting to achieve a translucent effect.
     */
    void draw() {
        // Enable blending to render transparent particles
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LIGHTING);

        // Draw each particle
        for (const auto& particle : particles) {
            glPushMatrix();
            glTranslatef(particle.position.x, particle.position.y, particle.position.z); // Move to particle's position

            // Set bubble-like material properties
            GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 0.3f }; // Transparent color
            GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 0.7f }; // Bright specular highlight
            GLfloat shininess[] = { 100.0f }; // Shiny surface

            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

            // Draw the particle as a sphere
            GLUquadric* quad = gluNewQuadric(); // Create a new quadratic object for drawing
            gluQuadricNormals(quad, GLU_SMOOTH); // Smooth shading for the sphere
            gluSphere(quad, particle.size * 0.5f, 16, 16); // Draw a sphere with radius based on particle size
            gluDeleteQuadric(quad); // Clean up the quadratic object

            glPopMatrix();
        }

        glDisable(GL_BLEND); // Disable blending after drawing
    }
};

#endif // PARTICLESYSTEM_H
