#pragma once

#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glut.h>
#include <imgui/imgui_impl_opengl2.h>

using namespace std;

#include "ObjectGL.h"
#include "Floor.h"
#include "Light.h"
//#include "Dog.h"
#include "Walls.h"
#include "ParticleSystem.h"
#include "Robot.h"

// window vars
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 900;
const int WINDOW_POS_X = 100;
const int WINDOW_POS_Y = 50;
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
static float aspect = WINDOW_RATIO;

// camera vars
static GLfloat camera_position[3] = { 6, 10, 20 };
static GLfloat camera_target[3] = { 0, 0, 0 };

// light
static float ambient_intensity = 0.5f;
static int fe = 0; // 0 if flashlight is enabled and 1 if disabled
static int fl = 0;
static int le = 0; // 0 if lamp is enabled and 1 if disabled


static bool vibratingSpeakers = true;
static bool vibratingAlien = true;


// others
static bool debug_mode = false;
static bool show_menu = true;
// dog
/*
static bool wag_tail = false;
static bool close_legs_hor = false;
static bool close_legs_vert = false;
static bool dog_view = false;
*/
static bool robot_view = false;

// imgui state
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

// this class represents the whole scene
class Scene
{
private:
	//Dog* dog;
	Robot robot;
	ObjectGL* desk;
	ObjectGL* speakers;
	ObjectGL* dj;
	ObjectGL* alien;
	ObjectGL* static_robot;
	ObjectGL* bubblesMachine;
	ObjectGL* chair;
	Light* flashlight;
	Light* lamp;
	Floor* floor;
	Walls* walls;
	ParticleSystem smokeSystem; // Add this line
	bool dragging = false;
	int lastMouseX = 0;
	int lastMouseY = 0;
	float cameraSpeed = 0.1f; // Adjust this value to change the camera movement speed
	bool fullScreen = false;
	int windowedX, windowedY, windowedWidth, windowedHeight;
	void drawRobot();
	void drawCoordinateArrows(); // draw coordinate arrows for debugging
	static Scene* currentInstance; // trick that helps to add OpenGL callbacks in class
	void display_menu(); // display the menu (based on ImGui)
	void addSmokeParticle(); // Function to add a smoke particle
public:
	Scene(int argc, char** argv);
	void display(); // Function where the scene drawing occurs	
	void keyboard(unsigned char key, int x, int y); // Function for keyboard press
	void keyboardUp(unsigned char key, int x, int y); // Function for keyboard after press
	void reshape(GLint w, GLint h); // Function to handle reshape of the screen
	void SpecialInput(int key, int x, int y); // Function to handle arrow keys
	void SpecialInputUp(int key, int x, int y); // Function to handle arrow keys after press
	void timer(int v); // Function that will be called every interval of time
	void mouseButton(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mouseWheel(int button, int dir, int x, int y);
	void loadFonts();
	void toggleFullScreen();



};
