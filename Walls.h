#pragma once
#include <GL/glut.h>
#include <string>
#include <glm/glm.hpp>
#include <array>

using namespace std;

class Walls
{
public:
    Walls(GLfloat height, GLfloat xMin = -10, GLfloat xMax = 10, GLfloat yMin = -10, GLfloat yMax = 10, int rows = 10, int columns = 10);
    void draw();
    ~Walls() = default;

    GLfloat xMin; // left wall x coordinate
    GLfloat xMax; // right wall x coordinate
    GLfloat yMin; // bottom wall y coordinate
    GLfloat yMax; // top wall y coordinate
    GLfloat height; // the height of the walls
    GLfloat alpha = 0.5f; // transparency
    GLfloat color[3] = { 0.5f, 0.78f, 0.884f }; // wall's color
    bool showNorth = false; // show top wall
    bool showSouth = true; // show bottom wall
    bool showEast = true; // show right wall
    bool showWest = true; // show left wall
    int rows; // number of rows to draw in each wall (for light)
    int columns; // number of columns to draw in each wall (for light)

private:
    std::array<GLfloat, 3> southWallColor;
    std::array<GLfloat, 3> northWallColor;
    std::array<GLfloat, 3> westWallColor;
    std::array<GLfloat, 3> eastWallColor;

    void generateWallColors();
};
