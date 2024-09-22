#pragma once
#include <GL/glut.h>
#include <vector>
#include <array>

class Floor
{
public:
    Floor(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows = 10, int columns = 10);
    void draw(); // draw the floor
    ~Floor() = default;

    GLfloat xMin; // floor's left x coordinate
    GLfloat xMax; // floor's right x coordinate
    GLfloat yMin; // floor's top y coordinate
    GLfloat yMax; // floor's bottom y coordinate


    int rows; // number of rows to draw in the floor
    int columns; // number of columns to draw in the floor

private:
    std::vector<std::vector<std::array<GLfloat, 3>>> tileColors; // to store the colors of the tiles
    void generateTileColors(); // to generate the colors of the tiles
};
