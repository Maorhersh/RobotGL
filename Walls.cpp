#include "Walls.h"
#include "RandomColor.h"

// Constructor for Walls
// Initializes wall dimensions, color, and random seed
Walls::Walls(GLfloat height, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
    // Set wall dimensions and position boundaries
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->height = height;
    this->rows = rows;
    this->columns = columns;

    // Initialize the random seed for color generation
    initRandomSeed();

    // Generate random colors for the walls
    generateWallColors();
}

// Generate random colors for each wall
void Walls::generateWallColors() {
    GLfloat r, g, b;

    // Generate and set color for the south wall
    generateRandomColor(r, g, b);
    southWallColor = { r, g, b };

    // Generate and set color for the north wall
    generateRandomColor(r, g, b);
    northWallColor = { r, g, b };

    // Generate and set color for the west wall
    generateRandomColor(r, g, b);
    westWallColor = { r, g, b };

    // Generate and set color for the east wall
    generateRandomColor(r, g, b);
    eastWallColor = { r, g, b };
}

// Draw the walls with the assigned colors
void Walls::draw() {
    glPushMatrix(); // Save the current matrix state

    glNormal3d(0, 1, 0); // Set normal vector for wall surfaces

    // Set material properties for walls
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f }; // White specular highlight
    GLfloat shininess = 64.0f; // Shininess of the material
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // Draw the south wall if enabled
    if (showSouth) {
        GLfloat currentColor[4] = { southWallColor[0], southWallColor[1], southWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glBegin(GL_QUADS);
        glVertex3f(xMin, 0, yMin);
        glVertex3f(xMax, 0, yMin);
        glVertex3f(xMax, height, yMin);
        glVertex3f(xMin, height, yMin);
        glEnd();
    }

    // Draw the north wall if enabled
    if (showNorth) {
        GLfloat currentColor[4] = { northWallColor[0], northWallColor[1], northWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glBegin(GL_QUADS);
        glVertex3f(xMin, 0, yMax);
        glVertex3f(xMax, 0, yMax);
        glVertex3f(xMax, height, yMax);
        glVertex3f(xMin, height, yMax);
        glEnd();
    }

    // Draw the west wall if enabled
    if (showWest) {
        GLfloat currentColor[4] = { westWallColor[0], westWallColor[1], westWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glBegin(GL_QUADS);
        glVertex3f(xMin, 0, yMin);
        glVertex3f(xMin, 0, yMax);
        glVertex3f(xMin, height, yMax);
        glVertex3f(xMin, height, yMin);
        glEnd();
    }

    // Draw the east wall if enabled
    if (showEast) {
        GLfloat currentColor[4] = { eastWallColor[0], eastWallColor[1], eastWallColor[2], 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

        glBegin(GL_QUADS);
        glVertex3f(xMax, 0, yMin);
        glVertex3f(xMax, 0, yMax);
        glVertex3f(xMax, height, yMax);
        glVertex3f(xMax, height, yMin);
        glEnd();
    }

    glPopMatrix(); // Restore the previous matrix state
}
