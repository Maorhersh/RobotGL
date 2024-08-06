#include "Floor.h"
#include "RandomColor.h"
#include <vector>

// Constructor
Floor::Floor(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
    this->xMax = xMax;
    this->xMin = xMin;
    this->yMax = yMax;
    this->yMin = yMin;
    this->rows = rows * 3; // Increase rows by 3 times
    this->columns = columns * 3; // Increase columns by 3 times

    // Initialize random seed
    initRandomSeed();

    // Generate tile colors
    generateTileColors();
}

void Floor::generateTileColors() {
    tileColors.resize(rows, std::vector<std::array<GLfloat, 3>>(columns));
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            GLfloat r, g, b;
            generateRandomColor(r, g, b);
            tileColors[row][column] = { r, g, b };
        }
    }
}

void Floor::draw() {
    glPushMatrix();
    glNormal3d(0, 1, 0);

    float width = xMax - xMin;
    float height = yMax - yMin;
    float row_step = height / (float)rows; // calculate row height
    float column_step = width / (float)columns; // calculate column width

    GLfloat specular[] = { 1.0f, 1.0f, 1.0f };
    GLfloat shininess = 128.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); // make the floor shiny

    // Draw the tiles
    glBegin(GL_QUADS);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            GLfloat r = tileColors[row][column][0];
            GLfloat g = tileColors[row][column][1];
            GLfloat b = tileColors[row][column][2];
            GLfloat currentColor[4] = { r, g, b, 1.0f }; // Set the stored color
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

            float x0 = xMin + column * column_step;
            float x1 = xMin + (column + 1) * column_step;
            float y0 = yMin + row * row_step;
            float y1 = yMin + (row + 1) * row_step;

            x0 = std::max(x0, xMin);
            x1 = std::min(x1, xMax);
            y0 = std::max(y0, yMin);
            y1 = std::min(y1, yMax);

            glVertex3f(x0, 0, y0);
            glVertex3f(x1, 0, y0);
            glVertex3f(x1, 0, y1);
            glVertex3f(x0, 0, y1);
        }
    }
    glEnd();

    // Draw the borders
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f); // Black color for borders
    glBegin(GL_LINES);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            float x0 = xMin + column * column_step;
            float x1 = xMin + (column + 1) * column_step;
            float y0 = yMin + row * row_step;
            float y1 = yMin + (row + 1) * row_step;

            x0 = std::max(x0, xMin);
            x1 = std::min(x1, xMax);
            y0 = std::max(y0, yMin);
            y1 = std::min(y1, yMax);

            // Draw the borders
            glVertex3f(x0, 0.01, y0);
            glVertex3f(x1, 0.01, y0);

            glVertex3f(x1, 0.01, y0);
            glVertex3f(x1, 0.01, y1);

            glVertex3f(x1, 0.01, y1);
            glVertex3f(x0, 0.01, y1);

            glVertex3f(x0, 0.01, y1);
            glVertex3f(x0, 0.01, y0);
        }
    }
    glEnd();

    glPopMatrix();
}
