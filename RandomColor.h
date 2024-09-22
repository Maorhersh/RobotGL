#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include <cstdlib>
#include <ctime>

// Initialize random seed for generating random colors
// This function ensures that the random number generator is seeded only once
// to avoid generating the same sequence of random numbers on each run.
inline void initRandomSeed() {
    static bool initialized = false; // Flag to ensure initialization happens only once
    if (!initialized) {
        std::srand(static_cast<unsigned int>(std::time(0))); // Seed the random number generator with the current time
        initialized = true; // Set the flag to true to prevent re-initialization
    }
}

/**
 * @brief Generate a random color.
 *
 * This function sets the provided RGB values to random colors. Each color component
 * is a floating-point value between 0.0 and 1.0.
 *
 * @param r Reference to the red component of the color.
 * @param g Reference to the green component of the color.
 * @param b Reference to the blue component of the color.
 */
inline void generateRandomColor(GLfloat& r, GLfloat& g, GLfloat& b) {
    r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // Generate random red component
    g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // Generate random green component
    b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // Generate random blue component
}

#endif // RANDOMCOLOR_H
