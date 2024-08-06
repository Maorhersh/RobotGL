#include "Walls.h"
#include "RandomColor.h"

// Constructor
Walls::Walls(GLfloat height, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->height = height;
    this->rows = rows;
    this->columns = columns;

    // Initialize random seed
    initRandomSeed();

    // Generate wall colors
    generateWallColors();
}

void Walls::generateWallColors() {
    GLfloat r, g, b;
    generateRandomColor(r, g, b);
    southWallColor = { r, g, b };

    generateRandomColor(r, g, b);
    northWallColor = { r, g, b };

    generateRandomColor(r, g, b);
    westWallColor = { r, g, b };

    generateRandomColor(r, g, b);
    eastWallColor = { r, g, b };
}

void Walls::draw() {
    glPushMatrix();
    glNormal3d(0, 1, 0);

    GLfloat specular[] = { 1.0f, 1.0f, 1.0f };
    GLfloat shininess = 64.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glBegin(GL_QUADS);
    if (showSouth) {
        GLfloat currentColor[4] = { southWallColor[0], southWallColor[1], southWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glVertex3f(xMin, 0, yMin);
        glVertex3f(xMax, 0, yMin);
        glVertex3f(xMax, height, yMin);
        glVertex3f(xMin, height, yMin);
    }
    if (showNorth) {
        GLfloat currentColor[4] = { northWallColor[0], northWallColor[1], northWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glVertex3f(xMin, 0, yMax);
        glVertex3f(xMax, 0, yMax);
        glVertex3f(xMax, height, yMax);
        glVertex3f(xMin, height, yMax);
    }
    if (showWest) {
        GLfloat currentColor[4] = { westWallColor[0], westWallColor[1], westWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glVertex3f(xMin, 0, yMin);
        glVertex3f(xMin, 0, yMax);
        glVertex3f(xMin, height, yMax);
        glVertex3f(xMin, height, yMin);
    }
    if (showEast) {
        GLfloat currentColor[4] = { eastWallColor[0], eastWallColor[1], eastWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glVertex3f(xMax, 0, yMin);
        glVertex3f(xMax, 0, yMax);
        glVertex3f(xMax, height, yMax);
        glVertex3f(xMax, height, yMin);
    }
    glEnd();

    glPopMatrix();
}
