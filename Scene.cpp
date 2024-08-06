#include "Scene.h"
#define M_PI 3.14159265358979323846

// creating callbacks to the class functions as described in 
// https://stackoverflow.com/questions/3589422/using-opengl-glutdisplayfunc-within-class
Scene* currentInstance;
void displaycallback() { currentInstance->display(); }
void reshapecallback(GLint w, GLint h) { currentInstance->reshape(w, h); }
void keyboardcallback(unsigned char key, int x, int y)
{
    currentInstance->keyboard(key, x, y);
}

void timercallback(int v) { currentInstance->timer(v); }

// Callback functions
void mouseButtonCallback(int button, int state, int x, int y) { currentInstance->mouseButton(button, state, x, y); }
void mouseMotionCallback(int x, int y) { currentInstance->mouseMotion(x, y); }
void mouseWheelCallback(int button, int dir, int x, int y) { currentInstance->mouseWheel(button, dir, x, y); }

// Helper function to check if the mouse is over an ImGui window
bool isMouseOverGui() {
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

// Mouse button event handler
void Scene::mouseButton(int button, int state, int x, int y) {
    ImGui_ImplGLUT_MouseFunc(button, state, x, y);

    if (isMouseOverGui()) {
        return; // Ignore mouse events if they are over the ImGui interface
    }

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            dragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            dragging = false;
        }
    }
}

void Scene::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault(); // Default font
    io.Fonts->AddFontFromFileTTF("fonts/Blueberry.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("fonts/VeraMono-Bold.ttf", 16.0f);
    io.Fonts->Build();
}

// Mouse motion event handler
void Scene::mouseMotion(int x, int y) {
    ImGui_ImplGLUT_MotionFunc(x, y);

    if (isMouseOverGui()) {
        return; // Ignore mouse events if they are over the ImGui interface
    }

    if (dragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        camera_position[0] += dx * cameraSpeed;
        camera_position[1] -= dy * cameraSpeed;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// Mouse wheel event handler
void Scene::mouseWheel(int button, int dir, int x, int y) {
    ImGui_ImplGLUT_MouseWheelFunc(button, dir, x, y);

    if (isMouseOverGui()) {
        return; // Ignore mouse events if they are over the ImGui interface
    }

    if (dir > 0) {
        camera_position[2] -= cameraSpeed * 10; // Zoom in
    }
    else {
        camera_position[2] += cameraSpeed * 10; // Zoom out
    }

    glutPostRedisplay();
}

void Scene::addSmokeParticle() {
    glm::vec3 pos(10.0f, 2.0f, 4.8f); // Starting position of the bubble

    // Increase horizontal velocity spread and decrease vertical velocity
    glm::vec3 vel(
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 5.0f, // Increased horizontal spread
        (static_cast<float>(rand()) / RAND_MAX) * 2.0f + 3.0f, // Reduced initial upward velocity
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 5.0f
    );

    // Transparent color
    glm::vec4 col(1.0f, 1.0f, 1.0f, 0.3f); // Transparent

    float life = 10.0f; // Lifespan for bubbles
    float size = static_cast<float>(rand()) / RAND_MAX * 0.1f + 0.2f; // Random size between 0.3 and 0.4

    GLfloat wallXMin = -12.0f;
    GLfloat wallXMax = 12.0f;
    GLfloat wallYMin = -12.0f;
    GLfloat wallYMax = 12.0f;
    GLfloat wallHeight = 12.0f;

    smokeSystem.addParticle(Particle(pos, vel, col, life, size, wallXMin, wallXMax, wallYMin, wallYMax, wallHeight));
}

Scene::Scene(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // Get the display size
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    // Set the window size to the display size
    glutInitWindowSize(screenWidth, screenHeight);

    // Set the window position to (0, 0)
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Fusturistic party");

    // Create drawing objects
    //this->dog = new Dog("GermanShephardLowPoly.obj", 0, 0, 0, 0.5f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0));
    this->floor = new Floor(-12, 12, -12, 12);
    this->walls = new Walls(12, -12, 12, -12, 12);
    this->alien = new ObjectGL("alien.obj", -8, 3.846, -8, 7.5f);
    this->alien->angle = 123;
    this->alien->initY = 3.846;
    this->desk = new ObjectGL("myDesk.obj", -5.3, 0, -5.3, 3.5f);
    this->desk->angle = 135;
    this->static_robot = new ObjectGL("ROBOT-TEX.obj", 5.4, 2.2, -9.5f, 5.0f);
    this->dj = new ObjectGL("dj.obj", -5.3, 3.9, -5.3, 3.5f);
    this->dj->angle = 40;
    this->bubblesMachine = new ObjectGL("smokeMachine.obj", 10.0, 1, 4.8, 1.2f);
    this->speakers = new ObjectGL("speakers.obj", -11, 0, 4.98, 0.16f);
    this->speakers->initY = 0;
    this->speakers->initY = 0;
    // Flashlight setup
    this->flashlight = new Light(GL_LIGHT0, 11, 11, -11, "spotlight.obj", 2.5f);
    this->flashlight->target[0] = -6; // Update target position
    this->flashlight->target[1] = -2; // Update target position
    this->flashlight->target[2] = 8.5; // Update target position
    this->flashlight->towardVector = glm::vec3(0, 0, 1); // Set direction to face the Z-axis

    // Lamp setup
    this->lamp = new Light(GL_LIGHT1, -11, 11, -11, "roundSpot.obj", 3.5f);
    this->lamp->target[0] = -13.3; // Update target position
    this->lamp->target[1] = 0; // If you want to adjust the y-coordinate
    this->lamp->target[2] = -15; // Update target position
    this->lamp->object->angle = 40; // Set rotation angle for the lamp's object representation
    robot = Robot();
    speakers->setVibration(vibratingSpeakers, speakers->PosY);
    alien->setVibration(vibratingAlien, alien->PosY);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Load fonts
    loadFonts();

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs(); // use the ImGui GLUT funcs
    ImGui_ImplOpenGL2_Init();

    // Configure GLUT callbacks
    ::currentInstance = this;
    glutReshapeFunc(reshapecallback);
    glutDisplayFunc(displaycallback);
    glutTimerFunc(100, timercallback, 0);
    glutKeyboardFunc(keyboardcallback);
    //glutKeyboardUpFunc(keyboardupcallback);
   // glutSpecialFunc(Specialcallback);
    //glutSpecialUpFunc(Specialupcallback);
    glutMouseFunc(mouseButtonCallback);
    glutMotionFunc(mouseMotionCallback);
    glutMouseWheelFunc(mouseWheelCallback);

    // Initialize random seed for smoke particles
    srand(static_cast<unsigned int>(time(0)));

    glutMainLoop(); // Run the main loop

    // ImGui cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}

void Scene::display() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    // display the menu with imgui
    if (show_menu)
        display_menu();

    // Rendering menu
    ImGui::Render();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1, 100.0); // use Perspective projection
    if (robot_view) {
        // Get the eye and center positions from the robot
        glm::vec3 viewPos = robot.getViewPos();  // Get the view position (eye and center)
        float eyeX = viewPos.x;
        float eyeY = viewPos.y;
        float eyeZ = viewPos.z;

        glm::vec3 viewVector = robot.getViewVector();  // Get the view direction vector
        float centerX = eyeX + viewVector.x;
        float centerY = eyeY + viewVector.y;
        float centerZ = eyeZ + viewVector.z;

        // Set the camera using gluLookAt
        gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
    }
    else {
        // Regular view mode
        gluLookAt(camera_position[0], camera_position[1], camera_position[2],
            camera_target[0], camera_target[1], camera_target[2],
            0, 1, 0);
    }


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // enable opengl features
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    // add scene lights
    flashlight->addlight();
    lamp->addlight();

    // ambient intensity
    GLfloat globalAmbientVec[4] = { ambient_intensity , ambient_intensity, ambient_intensity, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);
    glLoadIdentity();

    // start drawing
    floor->draw();
    alien->draw();
   // dog->draw();
    static_robot->draw();
    dj->draw();
    desk->draw();
    speakers->draw();
    bubblesMachine->draw();
    flashlight->draw();
    lamp->draw();
    robot.draw();

    // enable blending for walls transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    walls->draw();
    glDisable(GL_BLEND);

    // Update and draw the smoke system
    for (int i = 0; i < 5; ++i) {
        addSmokeParticle();
    }
    smokeSystem.update(0.1f); // Assuming 60 FPS, delta time is 1/60
    smokeSystem.draw();

    // add Coordinate Arrows for debug
    drawCoordinateArrows();
    // ImGui does not handle light well
    glDisable(GL_LIGHTING);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glEnable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::timer(int v) {

    smokeSystem.update(0.016f); // Assuming 60 FPS, delta time is 1/60
    //robot.walkLegs(0.1); // or any other transformation

    // Apply vibration to the selected objects
    static float time = 0.0f;
    time += 0.016f; // Increment time by the frame duration

    if (vibratingSpeakers) {
        speakers->vibrate(0.1f, 5.0f, 5*time,speakers->PosY);
    }

    if (vibratingAlien) {
        alien->vibrate(0.4f, 10.0f, 10*time, alien->PosY);
    }
  


    glutTimerFunc(1000 / 60, timercallback, v); // recursively call itself for the next frame
}

// in keyboard function
void Scene::keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.1f; // Movement speed

    switch (key) {
    case 'w': {
        float rad = glm::radians(robot.getDirection()); // Convert direction to radians
        float newX = robot.getPositionX() + moveSpeed * sin(rad);
        float newZ = robot.getPositionZ() + moveSpeed * cos(rad);
        std::cout << "Move Forward: Direction: " << robot.getDirection()
            << ", New Position: X=" << newX << ", Z=" << newZ << std::endl;
        robot.setPosition(newX, robot.getPositionY(), newZ);
        robot.moveLegs();
        break;
    }
    case 's': {
        float rad = glm::radians(robot.getDirection()); // Convert direction to radians
        float newX = robot.getPositionX() - moveSpeed * sin(rad);
        float newZ = robot.getPositionZ() - moveSpeed * cos(rad);
        std::cout << "Move Backward: Direction: " << robot.getDirection()
            << ", New Position: X=" << newX << ", Z=" << newZ << std::endl;
        robot.setPosition(newX, robot.getPositionY(), newZ);
        robot.moveLegs();
        break;
    }
    case 'a': // rotate left
        robot.setDirection(robot.getDirection() - 5.0f);
        break;
    case 'd': // rotate right
        robot.setDirection(robot.getDirection() + 5.0f);
        break;

    case 't':
        robot.rotateHead(5.0, 0.0);
        break;
    case 'g':
        robot.rotateHead(-5.0, 0.0);
        break;
    case 'h':
        robot.rotateHead(0.0, 5.0);
        break;
    case 'f':
        robot.rotateHead(0.0, -5.0);
        break;

        // Left Arm Shoulder
    case 'z':
        robot.rotateArmShoulder(5.0, true);
        break;
    case 'x':
        robot.rotateArmShoulder(-5.0, true);
        break;

        // Left Arm Elbow
    case 'c':
        robot.rotateArmElbow(5.0, true);
        break;
    case 'v':
        robot.rotateArmElbow(-5.0, true);
        break;

        // Left Arm Wrist
    case 'b':
        robot.rotateArmWrist(5.0, true);
        break;
    case 'n':
        robot.rotateArmWrist(-5.0, true);
        break;

        // Right Arm Shoulder
    case 'y':
        robot.rotateArmShoulder(5.0, false);
        break;
    case 'u':
        robot.rotateArmShoulder(-5.0, false);
        break;

        // Right Arm Elbow
    case 'i':
        robot.rotateArmElbow(5.0, false);
        break;
    case 'o':
        robot.rotateArmElbow(-5.0, false);
        break;

        // Right Arm Wrist
    case 'p':
        robot.rotateArmWrist(5.0, false);
        break;
    case '[':
        robot.rotateArmWrist(-5.0, false);
        break;

    case '0':
        robot_view = !robot_view;
        break;

    case '1': // Look up
        robot.rotateHead(10.0, 0.0); // Adjust the vertical head angle
        break;
    case '2': // Look down
        robot.rotateHead(-10.0, 0.0); // Adjust the vertical head angle
        break;
    case '3': // Look left
        robot.rotateHead(0.0, -10.0); // Adjust the horizontal head angle
        break;
    case '4': // Look right
        robot.rotateHead(0.0, 10.0); // Adjust the horizontal head angle
        break;
    }
    glutPostRedisplay();
}





void Scene::toggleFullScreen() {
    if (fullScreen) {
        glutReshapeWindow(windowedWidth, windowedHeight);
        glutPositionWindow(windowedX, windowedY);
    }
    else {
        windowedX = glutGet(GLUT_WINDOW_X);
        windowedY = glutGet(GLUT_WINDOW_Y);
        windowedWidth = glutGet(GLUT_WINDOW_WIDTH);
        windowedHeight = glutGet(GLUT_WINDOW_HEIGHT);
        glutFullScreen();
    }
    fullScreen = !fullScreen;
}



void Scene::reshape(GLint w, GLint h) {
    // imgui reshape func
    ImGui_ImplGLUT_ReshapeFunc(w, h);

    glViewport(0, 0, w, h);
    aspect = float(w) / float(h);
}



void Scene::drawCoordinateArrows(void) {
    if (!debug_mode) {
        return;
    }

    // draw lines
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    // draw letters
    glRasterPos3f(1.2f, 0.0f, 0.0f);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');
    glRasterPos3f(0.0f, 1.2f, 0.0f);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
    glRasterPos3f(0.0f, 0.0f, 1.2f);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');
}

void Scene::display_menu() {
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font1 = io.Fonts->Fonts[1];
    ImFont* font2 = io.Fonts->Fonts[2];
    // Desired window size
    ImVec2 window_size = ImVec2(400, 600); // Modify to your desired menu window size

    // Calculate the center position
    ImVec2 window_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
        (io.DisplaySize.y - window_size.y) * 0.5f);

    // Set the next window position and size
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

    ImGui::PushFont(font2); // Set font1 as the default font for all ImGui elements

    ImGui::Begin("Menu", &show_menu);
    ImGui::Checkbox("debug mode", &debug_mode); HelpMarker("mark to enter debug mode");
    // Add a checkbox to toggle full screen mode
    static bool fullScreenChecked = fullScreen;
    ImGui::Separator();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Full Screen").x) * 0.5f);
    if (ImGui::Checkbox("Full Screen", &fullScreenChecked)) {
        toggleFullScreen();
    }

    if (ImGui::CollapsingHeader("Room")) {
        if (debug_mode) {
            if (ImGui::TreeNode("static robot")) {
                ImGui::SliderFloat("robot scale", &this->static_robot->scale, 0.0f, 3.0f);
                ImGui::SliderFloat("robot pos x", &this->static_robot->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("robot pos y", &this->static_robot->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("robot pos z", &this->static_robot->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("robot angle", &this->static_robot->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("desk")) {
                ImGui::SliderFloat("desk scale", &this->desk->scale, 0.0f, 3.0f);
                ImGui::SliderFloat("desk pos x", &this->desk->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("desk pos y", &this->desk->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("desk pos z", &this->desk->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("desk angle", &this->desk->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("bubbles Machine")) {
                ImGui::SliderFloat("bubbles Machine scale", &this->bubblesMachine->scale, 0.0f, 0.1f);
                ImGui::SliderFloat("bubbles Machine pos x", &this->bubblesMachine->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("bubbles Machine pos y", &this->bubblesMachine->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("bubbles Machine pos z", &this->bubblesMachine->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("bubbles Machine angle", &this->bubblesMachine->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("speakers")) {
                ImGui::SliderFloat("speakers scale", &this->speakers->scale, 0.0f, 0.1f);
                ImGui::SliderFloat("speakers pos x", &this->speakers->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("speakers pos y", &this->speakers->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("speakers pos z", &this->speakers->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("speakers angle", &this->speakers->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("alien")) {
                ImGui::SliderFloat("alien scale", &this->alien->scale, 0.0f, 0.1f);
                ImGui::SliderFloat("alien pos x", &this->alien->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("alien pos y", &this->alien->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("alien pos z", &this->alien->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("alien angle", &this->alien->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("chair")) {
                ImGui::SliderFloat("chair scale", &this->chair->scale, 0.0f, 5.0f);
                ImGui::SliderFloat("chair pos x", &this->chair->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("chair pos y", &this->chair->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("chair pos z", &this->chair->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("chair angle", &this->chair->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("dj")) {
                ImGui::SliderFloat("dj scale", &this->dj->scale, 0.0f, 0.05f);
                ImGui::SliderFloat("dj pos x", &this->dj->PosX, -10.0f, 10.0f);
                ImGui::SliderFloat("dj pos y", &this->dj->PosY, -10.0f, 10.0f);
                ImGui::SliderFloat("dj pos z", &this->dj->PosZ, -10.0f, 10.0f);
                ImGui::SliderFloat("dj angle", &this->dj->angle, 0.0f, 360.0f);
                ImGui::TreePop();
            }
        }

        if (debug_mode) {
            ImGui::Checkbox("show south wall", &walls->showSouth);
            ImGui::Checkbox("show north wall", &walls->showNorth);
            ImGui::Checkbox("show east wall", &walls->showEast);
            ImGui::Checkbox("show west wall", &walls->showWest);
        }
    }
    ImGui::Separator();


    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat("camera position x", &camera_position[0], -60.0f, 60.0f); HelpMarker("the x coordinate of the camera position");
        ImGui::SliderFloat("camera position y", &camera_position[1], -60.0f, 60.0f); HelpMarker("the y coordinate of the camera position");
        ImGui::SliderFloat("camera position z", &camera_position[2], -60.0f, 60.0f); HelpMarker("the z coordinate of the camera position");
        ImGui::SliderFloat("camera target x", &camera_target[0], -60.0f, 60.0f); HelpMarker("the x coordinate of the position that the camera will look at");
        ImGui::SliderFloat("camera target y", &camera_target[1], -60.0f, 60.0f); HelpMarker("the y coordinate of the position that the camera will look at");
        ImGui::SliderFloat("camera target z", &camera_target[2], -60.0f, 60.0f); HelpMarker("the z coordinate of the position that the camera will look at");
    }
    ImGui::Separator();


    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::RadioButton("enable flashlight", &fe, 0); ImGui::SameLine();
        ImGui::RadioButton("disable flashlight", &fe, 1); HelpMarker("enable to add the flashlight's light to the scene");
        ImGui::RadioButton("disable flicker", &fl, 0); ImGui::SameLine();
        ImGui::RadioButton("enable flicker", &fl, 1); HelpMarker("enable to add the flickering effect to the flashlight's light");
        if (fl == 1) {
            flashlight->enableFlicker();
        }
        else {
            flashlight->disableFlicker();
        }
        if (fe == 0) {
            flashlight->enable();
        }
        else {
            flashlight->disable();
        }
        ImGui::RadioButton("enable lamp", &le, 0); ImGui::SameLine();
        ImGui::RadioButton("disable lamp", &le, 1); HelpMarker("enable to add the lamp's light to the scene");
        if (le == 0) {
            lamp->enable();
        }
        else {
            lamp->disable();
        }
        ImGui::SliderFloat("adjust ambient light", &ambient_intensity, 0.0f, 1.0f); HelpMarker("control the intensity of the global ambient");
        ImGui::ColorEdit3("lamp light color", (float*)(&this->lamp->color)); HelpMarker("choose the color of the lamp's light");
        ImGui::ColorEdit3("flashlight light color", (float*)(&this->flashlight->color)); HelpMarker("choose the color of the flashlight's light");
        ImGui::SliderFloat("flashlight position x", &this->flashlight->position[0], -12.0f, 12.0f); HelpMarker("the x coordinate of the flashlight position");
        ImGui::SliderFloat("flashlight position y", &this->flashlight->position[1], 0.0f, 10.0f); HelpMarker("the y coordinate of the flashlight position");
        ImGui::SliderFloat("flashlight position z", &this->flashlight->position[2], -12.0f, 12.0f); HelpMarker("the x coordinate of the flashlight position");
        ImGui::SliderFloat("flashlight target x", &this->flashlight->target[0], -12.0f, 12.0f); HelpMarker("the x coordinate of the position that the flashlight will look at");
        ImGui::SliderFloat("flashlight target y", &this->flashlight->target[1], 0.0f, 12.0f); HelpMarker("the y coordinate of the position that the flashlight will look at");
        ImGui::SliderFloat("flashlight target z", &this->flashlight->target[2], -12.0f, 12.0f); HelpMarker("the z coordinate of the position that the flashlight will look at");
        ImGui::SliderFloat("flashlight cutoff", &this->flashlight->cutoff, 0.0f, 180.0f); HelpMarker("the angle that the flashlight's light is effective");
        ImGui::SliderFloat("flashlight exponent", &this->flashlight->exponent, 0.0f, 30.0f); HelpMarker("the intensity distribution of the flashlight's light");

        if (debug_mode) {
            ;

            ImGui::SliderFloat("lamp scale", &this->lamp->object->scale, 0.0f, 10.0f);
            ImGui::SliderFloat("lamp angle", &this->lamp->object->angle, 0.0f, 360.0f);
            ImGui::SliderFloat("lamp position x", &this->lamp->position[0], -17.0f, 17.0f); HelpMarker("the x coordinate of the lamp position");
            ImGui::SliderFloat("lamp position y", &this->lamp->position[1], -17.0f, 17.0f); HelpMarker("the y coordinate of the lamp position");
            ImGui::SliderFloat("lamp position z", &this->lamp->position[2], -17.0f, 17.0f); HelpMarker("the z coordinate of the lamp position");
            ImGui::SliderFloat("lamp target x", &this->lamp->target[0], -17.0f, 17.0f); HelpMarker("the x coordinate of the position that the lamp will look at");
            ImGui::SliderFloat("lamp target y", &this->lamp->target[1], -17.0f, 17.0f); HelpMarker("the y coordinate of the position that the lamp will look at");
            ImGui::SliderFloat("lamp target z", &this->lamp->target[2], -17.0f, 17.0f); HelpMarker("the z coordinate of the position that the lamp will look at");
            ImGui::SliderFloat("lamp cutoff", &this->lamp->cutoff, 0.0f, 180.0f); HelpMarker("the angle that the lamp's light is effective");
            ImGui::SliderFloat("lamp exponent", &this->lamp->exponent, 0.0f, 50.0f); HelpMarker("the intensity distribution of the lamp's light");
        }
    }
    ImGui::Separator();


    if (ImGui::CollapsingHeader("Effects")) {
        ImGui::Checkbox("Vibrate Speakers", &vibratingSpeakers);
        ImGui::Checkbox("Vibrate Alien", &vibratingAlien);

        // Enable vibration for the selected objects
        speakers->setVibration(vibratingSpeakers, speakers->PosY);
        alien->setVibration(vibratingAlien, alien->PosY);
    }
    ImGui::Separator();


    if (ImGui::CollapsingHeader("help")) {
        ImGui::Text("Welcome to my opengl dog's room project");
        ImGui::TextWrapped("you can go in the room and explore the objects, you can also use this menu to affect the room.");
        ImGui::TextWrapped("normally you can see some basic options and with debug mode you almost full control on the scene");
        ImGui::Separator();
        ImGui::Text("ROOM:");
        ImGui::TextWrapped("this category affect the room and it's object, for example you can change the walls and the floor color");
        ImGui::Separator();
        ImGui::Text("CAMERA:");
        ImGui::TextWrapped("this category affect the camera, you can change the settings to see the room from different angles");
        ImGui::Separator();
        ImGui::Text("DOG:");
        ImGui::TextWrapped("this category affect our hero, the dog, you can move it's head and tail.");
        ImGui::TextWrapped("the main feature here is to see the world from the dog view");
        ImGui::Separator();
        ImGui::Text("LIGHTS:");
        ImGui::TextWrapped("this category affect the lights' the flashlight and the lamp.");
        ImGui::TextWrapped("you can control the light settings and see how it's affect the objects");
        ImGui::Separator();
        ImGui::Text("the available keyboards:");
        ImGui::Columns(2, "keyboard");
        ImGui::Separator();
        ImGui::Text("Key"); ImGui::NextColumn();
        ImGui::Text("Action"); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("'w' / up arrow"); ImGui::NextColumn(); ImGui::Text("move the dog forward"); ImGui::NextColumn();
        ImGui::Text("'s' / down arrow"); ImGui::NextColumn(); ImGui::Text("move the dog backward"); ImGui::NextColumn();
        ImGui::Text("'d' / right arrow"); ImGui::NextColumn(); ImGui::Text("turn the dog right"); ImGui::NextColumn();
        ImGui::Text("'a' / left arrow"); ImGui::NextColumn(); ImGui::Text("turn the dog left"); ImGui::NextColumn();
        ImGui::Text("'q'"); ImGui::NextColumn(); ImGui::Text("turn the dog head left"); ImGui::NextColumn();
        ImGui::Text("'e'"); ImGui::NextColumn(); ImGui::Text("turn the dog head right"); ImGui::NextColumn();
        ImGui::Text("'g'"); ImGui::NextColumn(); ImGui::Text("turn the dog head down"); ImGui::NextColumn();
        ImGui::Text("'t'"); ImGui::NextColumn(); ImGui::Text("turn the dog head up"); ImGui::NextColumn();
        ImGui::Text("'m'"); ImGui::NextColumn(); ImGui::Text("show or hide menu"); ImGui::NextColumn();
        ImGui::Text("'v'"); ImGui::NextColumn(); ImGui::Text("change view (between camera and dog)"); ImGui::NextColumn();
        ImGui::Columns(1);
    }

    if (debug_mode) {
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5f);
    ImGui::Indent(ImGui::GetWindowWidth() * 0.25f);
    ImGui::Separator();


    if (ImGui::Button("EXIT", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
        ImGui::OpenPopup("Exit?");
    }
    ImGui::PopStyleColor(3);

    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("do you sure you want to exit?\n\n");
        ImGui::Separator();

        if (ImGui::Button("yes", ImVec2(120, 0))) {
            glutLeaveMainLoop();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("no", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();

    ImGui::PopFont(); // Restore the previous font
}

