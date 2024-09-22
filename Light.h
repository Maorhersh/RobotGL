#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/vector_angle.hpp >
#include <algorithm>
#include <iterator>
#include <vector>
using namespace std;

#include "ObjectGL.h"

class Light {
public:
    GLfloat position[4]; // the position of the light
    GLfloat target[3]; // where the light point to
    GLfloat color[3] = { 1.0f, 1.0f, 1.0f }; // the color of the light
    GLfloat cutoff; // the angle that the light is affective
    GLfloat exponent; // the intensity distribution of the light
    glm::vec3 towardVector = glm::vec3(0, -1, 0);  // the direction of the light drawing

    ObjectGL* object = NULL; // object to draw

    Light(int id, GLfloat PosX = 0, GLfloat PosY = 10, GLfloat PosZ = 0, string object = "",
        GLfloat scale = 1.0f, GLfloat cutoff = 90.0f, GLfloat exponent = 0.0f,
        GLfloat TargetX = 0, GLfloat TargetY = 0, GLfloat TargetZ = 0);
    void draw(); // draw the light in the scene
    void addlight(); // add the lighting of the light
    void disable(); // disable the light
    void enable(); // enable the light
    void enableFlicker(); // Enable flicker effect
    void disableFlicker(); // Disable flicker effect
    void updateFlicker(); // update the light color for the flicker effect
    ~Light() = default;

private:
    int id; // must be GL_LIGHTi where 0 <= i < GL_MAX_LIGHTS
    void fixDirection(); // rotate the light drawing to the wanted direction (from position to target)
    bool flicker = false; // flag to enable/disable flicker effect
    vector<glm::vec3> flickerColors; // colors for the flicker effect
    int flickerIndex = 0; // index for the current flicker color

    void generateFlickerColors(); // generate random colors for the flicker effect
};
