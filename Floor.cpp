#include "Floor.h"
#include "RandomColor.h"
#include <vector>

// Constructor for Floor
// Initializes the floor dimensions, tile grid size, and generates random tile colors
Floor::Floor(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
    // Set floor boundaries and dimensions
    this->xMax = xMax;
    this->xMin = xMin;
    this->yMax = yMax;
    this->yMin = yMin;
    this->rows = rows * 3; // Increase rows by a factor of 3 for more tiles
    this->columns = columns * 3; // Increase columns by a factor of 3 for more tiles

    // Initialize random seed for color generation
    initRandomSeed();

    // Generate random colors for each tile
    generateTileColors();
}

// Generate random colors for the tiles
void Floor::generateTileColors() {
    // Resize tileColors vector to hold color data for all tiles
    tileColors.resize(rows, std::vector<std::array<GLfloat, 3>>(columns));

    // Assign random colors to each tile
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            GLfloat r, g, b;
            generateRandomColor(r, g, b);
            tileColors[row][column] = { r, g, b };
        }
    }
}

// Draw the floor tiles and borders
void Floor::draw() {
    glPushMatrix(); // Save the current matrix state

    glNormal3d(0, 1, 0); // Set normal vector for floor tiles (upwards)

    float width = xMax - xMin;
    float height = yMax - yMin;
    float row_step = height / (float)rows; // Calculate step size for row height
    float column_step = width / (float)columns; // Calculate step size for column width

    // Set material properties for tiles
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f }; // White specular highlight
    GLfloat shininess = 128.0f; // Shininess of the material
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess); // Make tiles shiny

    // Draw the floor tiles
    glBegin(GL_QUADS);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            GLfloat r = tileColors[row][column][0];
            GLfloat g = tileColors[row][column][1];
            GLfloat b = tileColors[row][column][2];
            GLfloat currentColor[4] = { r, g, b, 1.0f }; // Set the color for the tile
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, currentColor);

            float x0 = xMin + column * column_step; // Calculate tile corner positions
            float x1 = xMin + (column + 1) * column_step;
            float y0 = yMin + row * row_step;
            float y1 = yMin + (row + 1) * row_step;

            // Ensure tile edges are within bounds
            x0 = std::max(x0, xMin);
            x1 = std::min(x1, xMax);
            y0 = std::max(y0, yMin);
            y1 = std::min(y1, yMax);

            // Define vertices for the current tile
            glVertex3f(x0, 0, y0);
            glVertex3f(x1, 0, y0);
            glVertex3f(x1, 0, y1);
            glVertex3f(x0, 0, y1);
        }
    }
    glEnd();

    // Draw the borders of the tiles
    glLineWidth(2.0f); // Set line width for borders
    glColor3f(1.0f, 1.0f, 1.0f); // White color for borders
    glBegin(GL_LINES);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            float x0 = xMin + column * column_step; // Calculate border positions
            float x1 = xMin + (column + 1) * column_step;
            float y0 = yMin + row * row_step;
            float y1 = yMin + (row + 1) * row_step;

            // Ensure border edges are within bounds
            x0 = std::max(x0, xMin);
            x1 = std::min(x1, xMax);
            y0 = std::max(y0, yMin);
            y1 = std::min(y1, yMax);

            // Draw borders around the tile
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

    glPopMatrix(); // Restore the previous matrix state
}
