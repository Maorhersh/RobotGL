// RandomColor.h
#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include <cstdlib>
#include <ctime>

// Initialize random seed once
inline void initRandomSeed() {
    static bool initialized = false;
    if (!initialized) {
        std::srand(static_cast<unsigned int>(std::time(0)));
        initialized = true;
    }
}

// Generate a random color
inline void generateRandomColor(GLfloat& r, GLfloat& g, GLfloat& b) {
    r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

#endif // RANDOMCOLOR_H
