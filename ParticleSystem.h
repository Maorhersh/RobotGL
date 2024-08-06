#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <GL/glu.h> // Include GLU for drawing spheres

class ParticleSystem {
public:
    std::vector<Particle> particles;

    void addParticle(const Particle& particle) {
        particles.push_back(particle);
    }

    void update(float deltaTime) {
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }
        particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) { return !p.isAlive(); }), particles.end());
    }

    void draw() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LIGHTING);

        for (const auto& particle : particles) {
            glPushMatrix();
            glTranslatef(particle.position.x, particle.position.y, particle.position.z);

            // Set bubble-like material properties
            GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 0.3f }; // Transparent color
            GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 0.7f }; // Bright specular highlight
            GLfloat shininess[] = { 100.0f }; // Shiny surface

            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

            GLUquadric* quad = gluNewQuadric();
            gluQuadricNormals(quad, GLU_SMOOTH);
            gluSphere(quad, particle.size * 0.5f, 16, 16); // Draw a sphere with random size
            gluDeleteQuadric(quad);

            glPopMatrix();
        }

        glDisable(GL_BLEND);
    }
};

#endif // PARTICLESYSTEM_H
