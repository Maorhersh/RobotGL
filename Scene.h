#pragma once

#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glut.h>
#include <imgui/imgui_impl_opengl2.h>

using namespace std;

#include "ObjectGL.h"
#include "Floor.h"
#include "Light.h"
#include "Walls.h"
#include "ParticleSystem.h"
#include "Robot.h"
#include "Music.h"
#define M_PI 3.14159265358979323846

// Window settings
const int WINDOW_WIDTH = 1500;           // Width of the window
const int WINDOW_HEIGHT = 900;           // Height of the window
const int WINDOW_POS_X = 100;            // X position of the window on the screen
const int WINDOW_POS_Y = 50;             // Y position of the window on the screen
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT; // Aspect ratio of the window
static float aspect = WINDOW_RATIO;      // Aspect ratio used in rendering

// Camera settings
static GLfloat camera_position[3] = { 6, 10, 20 }; // Initial position of the camera
static GLfloat camera_target[3] = { 0, 0, 0 };    // Initial target of the camera (where it looks)

// Lighting settings
static float ambient_intensity = 0.5f;   // Ambient light intensity
static int fe = 0;                       // Rectangular spotlight enabled (0) or disabled (1)
static int fl = 0;                       // Unused but reserved for future light settings
static int le = 0;                       // Round spotlight enabled (0) or disabled (1)

// Scene interaction settings
static bool vibratingSpeakers = true;    // Toggle for vibrating speakers
static bool vibratingAlien = true;       // Toggle for vibrating alien
static bool dancingRobot = false;        // Toggle for dancing robot
static bool enableBubbles = true;        // Toggle for enabling bubbles (default is enabled)

// Miscellaneous settings
static bool debug_mode = false;          // Toggle for debug mode (shows additional information)
static bool show_menu = true;            // Toggle for displaying the ImGui menu
static bool robot_view = false;          // Toggle for robot's point of view

// ImGui helper function to show tooltips
static void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// This class represents the entire scene of the simulation
class Scene
{
private:
    // Scene objects
    Robot robot;                  // Main robot object
    ObjectGL* desk;               // Desk object
    ObjectGL* speakers;           // Speakers object
    ObjectGL* dj;                 // DJ object
    ObjectGL* alien;              // Alien object
    ObjectGL* static_robot;       // Static robot object
    ObjectGL* bubblesMachine;     // Bubbles machine object
    ObjectGL* chair;              // Chair object
    Light* rectSpotlight;         // Rectangular spotlight object
    Light* roundSpotlight;        // Round spotlight object
    Floor* floor;                 // Floor object
    Walls* walls;                 // Walls object
    ParticleSystem bubbles;   // Particle system for smoke

    // Interaction state
    bool dragging = false;        // State for mouse dragging
    int lastMouseX = 0;           // Last X position of the mouse
    int lastMouseY = 0;           // Last Y position of the mouse
    float cameraSpeed = 0.1f;     // Speed of the camera movement
    bool fullScreen = false;      // Full-screen toggle state
    int windowedX, windowedY, windowedWidth, windowedHeight; // Store windowed mode settings

    // Helper methods
    void drawRobot();             // Method to draw the robot
    void drawCoordinateArrows();  // Method to draw coordinate arrows for debugging
    static Scene* currentInstance; // Static instance to allow OpenGL callbacks in class
    void display_menu();          // Method to display the ImGui menu
    void addBubble();      // Method to add a smoke particle

public:
    // Constructor
    Scene(int argc, char** argv);

    // OpenGL callback methods
    void display();               // Method where the scene drawing occurs
    void keyboard(unsigned char key, int x, int y); // Method for keyboard press events
    void keyboardUp(unsigned char key, int x, int y); // Method for keyboard release events
    void reshape(GLint w, GLint h); // Method to handle window resizing
    void SpecialInput(int key, int x, int y); // Method to handle special key (arrow) press events
    void SpecialInputUp(int key, int x, int y); // Method to handle special key release events
    void timer(int v);            // Method that is called every time interval
    void mouseButton(int button, int state, int x, int y); // Method to handle mouse button events
    void mouseMotion(int x, int y); // Method to handle mouse motion events
    void mouseWheel(int button, int dir, int x, int y); // Method to handle mouse wheel events
    void loadFonts();             // Method to load fonts for ImGui
    void toggleFullScreen();      // Method to toggle between full-screen and windowed mode
};
