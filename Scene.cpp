#include "Scene.h"

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
    io.Fonts->AddFontFromFileTTF("fonts/VeraMono-Bold.ttf", 24.0f);
    io.Fonts->AddFontFromFileTTF("fonts/VeraMono-Bold.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("fonts/VeraMono-Bold.ttf", 20.0f);
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

void Scene::addBubble() {
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

    bubbles.addParticle(Particle(pos, vel, col, life, size, wallXMin, wallXMax, wallYMin, wallYMax, wallHeight));
}

Scene::Scene(int argc, char** argv) {
    // Initialize SDL and play the song
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL for audio" << std::endl;
        return;
    }

    startMusic("party.mp3");

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
    this->floor = new Floor(-12, 12, -12, 12);
    this->walls = new Walls(12, -12, 12, -12, 12);
    this->alien = new ObjectGL("alien.obj", -8, 3.846, -8, 7.5f);
    this->alien->angle = 123;
    this->alien->initY = 3.846;
    this->desk = new ObjectGL("myDesk.obj", -5.3, 0, -5.3, 3.5f);
    this->desk->angle = 135;
    this->static_robot = new ObjectGL("ROBOT-TEX.obj", 5.4, 2.2, -9.5f, 5.0f);
    this->dj = new ObjectGL("dj.obj", -5.3, 3.9, -5.3, 3.5f);
    this->dj->angle = 33.66;
    this->bubblesMachine = new ObjectGL("smokeMachine.obj", 10.0, 1, 4.8, 1.2f);
    this->speakers = new ObjectGL("speakers.obj", -10.4, 0, 4.98, 0.16f);
    this->speakers->initY = 0;
    this->speakers->initY = 0;
    // rectSpotlight setup
    this->rectSpotlight = new Light(GL_LIGHT0, 10.763, 10.5, -10.482, "spotlight.obj", 2.7f);
    this->rectSpotlight->target[0] = -6; // Update target position
    this->rectSpotlight->target[1] = -2; // Update target position
    this->rectSpotlight->target[2] = 8.5; // Update target position
    this->rectSpotlight->towardVector = glm::vec3(0, 0, 1); // Set direction to face the Z-axis

    // roundSpotlight setup
    this->roundSpotlight = new Light(GL_LIGHT1, -10.391, 9.813, -10.1, "roundSpot.obj", 3.5f);
    this->roundSpotlight->target[0] = -13.3; // Update target position
    this->roundSpotlight->target[1] = 0.415; // If you want to adjust the y-coordinate
    this->roundSpotlight->target[2] = -15; // Update target position
    this->roundSpotlight->object->angle = 40; // Set rotation angle for the roundSpotlight's object representation
    this->roundSpotlight->exponent = 1.22; // Set rotation angle for the roundSpotlight's object representation
    this->roundSpotlight->cutoff = 71.7; // Set rotation angle for the roundSpotlight's object representation

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
    cleanUpSDL();

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
    rectSpotlight->addlight();
    roundSpotlight->addlight();

    // ambient intensity
    GLfloat globalAmbientVec[4] = { ambient_intensity , ambient_intensity, ambient_intensity, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);
    glLoadIdentity();

    // start drawing
    floor->draw();
    alien->draw();
    static_robot->draw();
    dj->draw();
    desk->draw();
    speakers->draw();
    bubblesMachine->draw();
    rectSpotlight->draw();
    roundSpotlight->draw();
    robot.draw();

    // enable blending for walls transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    walls->draw();
    glDisable(GL_BLEND);

    // Conditionally update and draw the smoke system (bubbles)
    if (enableBubbles) {
        for (int i = 0; i < 5; ++i) {
            addBubble();
        }
        bubbles.update(0.1f); // Assuming 60 FPS, delta time is 1/60
        bubbles.draw();
    }

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

    bubbles.update(0.016f); // Assuming 60 FPS, delta time is 1/60

    // Apply vibration to the selected objects
    static float time = 0.0f;
    time += 0.016f; // Increment time by the frame duration

    if (vibratingSpeakers) {
        speakers->vibrate(0.1f, 5.0f, 5*time,speakers->PosY);
    }

    if (vibratingAlien) {
        alien->vibrate(0.4f, 2.0f, 5*time, alien->PosY);
    }
  
    if (dancingRobot) {
        robot.dance(40*time);
    }


    glutTimerFunc(100 / 60, timercallback, v); // recursively call itself for the next frame
}

// in keyboard function
void Scene::keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.2f; // Movement speed

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
        robot.rotateHead(5.0, 0.0); // Adjust the vertical head angle
        break;
    case '2': // Look down
        robot.rotateHead(-5.0, 0.0); // Adjust the vertical head angle
        break;
    case '3': // Look left
        robot.rotateHead(0.0, -5.0); // Adjust the horizontal head angle
        break;
    case '4': // Look right
        robot.rotateHead(0.0, 5.0); // Adjust the horizontal head angle
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
    ImFont* font3 = io.Fonts->Fonts[3];

    // Desired window size
    ImVec2 window_size = ImVec2(460, 550); // Modify to your desired menu window size

    // Calculate the center position
    ImVec2 window_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
        (io.DisplaySize.y - window_size.y) * 0.5f);

    // Set the next window position and size
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

    ImGui::PushFont(font1); // Set font1 as the default font for all ImGui elements

    ImGui::Begin("Menu", &show_menu);
    //ImGui::Checkbox("debug mode", &debug_mode); HelpMarker("mark to enter debug mode");
    // Add a checkbox to toggle full screen mode
    static bool fullScreenChecked = fullScreen;


    ImGui::PushFont(font1); // Set font1 as the default font for all ImGui elements

    // Centering each text line
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Futristic Party").x) * 0.5f);
    ImGui::Text("Futristic Party");
    ImGui::Separator();
    ImGui::PopFont();
    ImGui::PushFont(font2); // Set font1 as the default font for all ImGui elements


    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat("camera location x", &camera_position[0], -60.0f, 60.0f); HelpMarker("the x coordinate of the camera location");
        ImGui::SliderFloat("camera location y", &camera_position[1], -60.0f, 60.0f); HelpMarker("the y coordinate of the camera location");
        ImGui::SliderFloat("camera location z", &camera_position[2], -60.0f, 60.0f); HelpMarker("the z coordinate of the camera location");
        ImGui::SliderFloat("camera target x", &camera_target[0], -60.0f, 60.0f); HelpMarker("the x coordinate of the location that the camera will look at");
        ImGui::SliderFloat("camera target y", &camera_target[1], -60.0f, 60.0f); HelpMarker("the y coordinate of the location that the camera will look at");
        ImGui::SliderFloat("camera target z", &camera_target[2], -60.0f, 60.0f); HelpMarker("the z coordinate of the location that the camera will look at");
    }
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Lights")) {

        ImGui::SliderFloat("rect Spotlight location x", &this->rectSpotlight->position[0], -12.0f, 12.0f); HelpMarker("the x coordinate of the rect Spotlight location");
        ImGui::SliderFloat("rect Spotlight location y", &this->rectSpotlight->position[1], 0.0f, 10.0f); HelpMarker("the y coordinate of the rect Spotlight location");
        ImGui::SliderFloat("rect Spotlight location z", &this->rectSpotlight->position[2], -12.0f, 12.0f); HelpMarker("the x coordinate of the rect Spotlight location");
        ImGui::SliderFloat("rect Spotlight target x", &this->rectSpotlight->target[0], -12.0f, 12.0f); HelpMarker("the x coordinate of the location that the rect Spotlight will look at");
        ImGui::SliderFloat("rect Spotlight target y", &this->rectSpotlight->target[1], 0.0f, 12.0f); HelpMarker("the y coordinate of the location that the rect Spotlight will look at");
        ImGui::SliderFloat("rect Spotlight target z", &this->rectSpotlight->target[2], -12.0f, 12.0f); HelpMarker("the z coordinate of the location that the rect potlight will look at");
        ImGui::SliderFloat("rect Spotlight cutoff", &this->rectSpotlight->cutoff, 0.0f, 180.0f); HelpMarker("the angle that the rect Spotlight's light is effective");
        ImGui::SliderFloat("rect Spotlight exponent", &this->rectSpotlight->exponent, 0.0f, 30.0f); HelpMarker("the intensity distribution of the rect Spotlight's light");
        ImGui::SliderFloat("round Spotlight scale", &this->roundSpotlight->object->scale, 0.0f, 10.0f);
        ImGui::SliderFloat("round Spotlight angle", &this->roundSpotlight->object->angle, 0.0f, 360.0f);
        ImGui::SliderFloat("round Spotlight location x", &this->roundSpotlight->position[0], -17.0f, 17.0f); HelpMarker("the x coordinate of the round Spotlight location");
        ImGui::SliderFloat("round Spotlight location y", &this->roundSpotlight->position[1], -17.0f, 17.0f); HelpMarker("the y coordinate of the round Spotlight location");
        ImGui::SliderFloat("round Spotlight location z", &this->roundSpotlight->position[2], -17.0f, 17.0f); HelpMarker("the z coordinate of the round Spotlight location");
        ImGui::SliderFloat("round Spotlight target x", &this->roundSpotlight->target[0], -17.0f, 17.0f); HelpMarker("the x coordinate of the location that the round Spotlight will look at");
        ImGui::SliderFloat("round Spotlight target y", &this->roundSpotlight->target[1], -17.0f, 17.0f); HelpMarker("the y coordinate of the location that the round Spotlight will look at");
        ImGui::SliderFloat("round Spotlight target z", &this->roundSpotlight->target[2], -17.0f, 17.0f); HelpMarker("the z coordinate of the location that the round Spotlight will look at");
        ImGui::SliderFloat("round Spotlight cutoff", &this->roundSpotlight->cutoff, 0.0f, 180.0f); HelpMarker("the angle that the round Spotlight's light is effective");
        ImGui::SliderFloat("round Spotlight exponent", &this->roundSpotlight->exponent, 0.0f, 50.0f); HelpMarker("the intensity distribution of the round Spotlight's light");
        ImGui::RadioButton("enable rect Spotlight", &fe, 0); ImGui::SameLine();
        ImGui::RadioButton("disable rect Spotlight", &fe, 1); HelpMarker("enable to add the rect Spotlight's light to the scene");
        ImGui::RadioButton("disable flicker", &fl, 0); ImGui::SameLine();
        ImGui::RadioButton("enable flicker", &fl, 1); HelpMarker("enable to add the flickering effect to the rectSpotlight's light");
        if (fl == 1) {
            rectSpotlight->enableFlicker();
        }
        else {
            rectSpotlight->disableFlicker();
        }
        if (fe == 0) {
            rectSpotlight->enable();
        }
        else {
            rectSpotlight->disable();
        }
        ImGui::RadioButton("enable round spot light", &le, 0); ImGui::SameLine();
        ImGui::RadioButton("disable round spot light", &le, 1); HelpMarker("enable to add the roundSpotlight's light to the scene");
        if (le == 0) {
            roundSpotlight->enable();
        }
        else {
            roundSpotlight->disable();
        }
        ImGui::SliderFloat("adjust ambient light", &ambient_intensity, 0.0f, 1.0f); HelpMarker("control the intensity of the global ambient");
        ImGui::ColorEdit3("round spot light light color", (float*)(&this->roundSpotlight->color)); HelpMarker("choose the color of the round spot light's light");
        ImGui::ColorEdit3("rect Spotlight light color", (float*)(&this->rectSpotlight->color)); HelpMarker("choose the color of the rect Spotlight's light");

    }
    ImGui::Separator();


    if (ImGui::CollapsingHeader("Effects")) {
        ImGui::Checkbox("control Vibrating Speakers", &vibratingSpeakers);
        ImGui::Checkbox("control jumping alien", &vibratingAlien);
        ImGui::Checkbox("dancing Robot", &dancingRobot);
        ImGui::Checkbox("control Bubbles", &enableBubbles);

        // Enable vibration for the selected objects
        speakers->setVibration(vibratingSpeakers, speakers->PosY);
        alien->setVibration(vibratingAlien, alien->PosY);

    }
    ImGui::Separator();

    ImGui::Separator();


    if (ImGui::CollapsingHeader("How to control the scene ? ")) {
        ImGui::Text("In this menu you have 3 options to choose from for scene control");
        ImGui::Separator();
 
        ImGui::Text("Camera:");
        ImGui::TextWrapped("When you open this tab You can adust location and target");
        ImGui::TextWrapped("but it also can be done by mouse drag and mouse wheel scrolling, try and see what happens :)");
        ImGui::Separator();
        ImGui::Text("Lights:");
        ImGui::TextWrapped("When you open this tab You can adust location and target  of rect spot light and the round spot light.");
        ImGui::TextWrapped("you can also adjust the light strength and color by changing the rest of options");
        ImGui::Separator();
        ImGui::Text("Effects:");
        ImGui::TextWrapped("When you open this tab You can control vibration of speakers and jumping of the alien");
        ImGui::Separator();
        ImGui::Text("keyboared conrollers");
        ImGui::Columns(2, "keyboard");
        ImGui::Separator();
        ImGui::Text("Key"); ImGui::NextColumn();
        ImGui::Text("Action"); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("'w'"); ImGui::NextColumn(); ImGui::Text("Move the robot forward"); ImGui::NextColumn();
        ImGui::Text("'s'"); ImGui::NextColumn(); ImGui::Text("Move the robot backward"); ImGui::NextColumn();
        ImGui::Text("'a'"); ImGui::NextColumn(); ImGui::Text("Turn the robot left"); ImGui::NextColumn();
        ImGui::Text("'d'"); ImGui::NextColumn(); ImGui::Text("Turn the robot right"); ImGui::NextColumn();
        ImGui::Text("'t'"); ImGui::NextColumn(); ImGui::Text("Turn the robot head up"); ImGui::NextColumn();
        ImGui::Text("'g'"); ImGui::NextColumn(); ImGui::Text("Turn the robot head down"); ImGui::NextColumn();
        ImGui::Text("'h'"); ImGui::NextColumn(); ImGui::Text("Turn the robot head left"); ImGui::NextColumn();
        ImGui::Text("'f'"); ImGui::NextColumn(); ImGui::Text("Turn the robot head right"); ImGui::NextColumn();
        ImGui::Text("'z'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm shoulder up"); ImGui::NextColumn();
        ImGui::Text("'x'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm shoulder down"); ImGui::NextColumn();
        ImGui::Text("'c'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm elbow up"); ImGui::NextColumn();
        ImGui::Text("'v'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm elbow down"); ImGui::NextColumn();
        ImGui::Text("'b'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm wrist up"); ImGui::NextColumn();
        ImGui::Text("'n'"); ImGui::NextColumn(); ImGui::Text("Rotate left arm wrist down"); ImGui::NextColumn();
        ImGui::Text("'y'"); ImGui::NextColumn(); ImGui::Text("Rotate right arm shoulder up"); ImGui::NextColumn();
        ImGui::Text("'u'"); ImGui::NextColumn(); ImGui::Text("Rotate right arm shoulder down"); ImGui::NextColumn();
        ImGui::Text("'i'"); ImGui::NextColumn(); ImGui::Text("Rotate right arm elbow up"); ImGui::NextColumn();
        ImGui::Text("'o'"); ImGui::NextColumn(); ImGui::Text("Rotate right arm elbow down"); ImGui::NextColumn();
        ImGui::Text("'p'"); ImGui::NextColumn(); ImGui::Text("Rotate right arm wrist up"); ImGui::NextColumn();
        ImGui::Text("'['"); ImGui::NextColumn(); ImGui::Text("Rotate right arm wrist down"); ImGui::NextColumn();
        ImGui::Text("'0'"); ImGui::NextColumn(); ImGui::Text("Toggle view (camera/robot)"); ImGui::NextColumn();
        ImGui::Text("'1'"); ImGui::NextColumn(); ImGui::Text("Look up"); ImGui::NextColumn();
        ImGui::Text("'2'"); ImGui::NextColumn(); ImGui::Text("Look down"); ImGui::NextColumn();
        ImGui::Text("'3'"); ImGui::NextColumn(); ImGui::Text("Look left"); ImGui::NextColumn();
        ImGui::Text("'4'"); ImGui::NextColumn(); ImGui::Text("Look right"); ImGui::NextColumn();

        ImGui::Columns(1); // Restore to single column layout
    }
    ImGui::Dummy(ImVec2(0.0f, 25.0f));  // 50 pixels of vertical spacing
    ImGui::PopFont();
    ImGui::PushFont(font1); // Set font1 as the default font for all ImGui elements

    // Centering each text line
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Welcome to the best party in town!").x) * 0.5f);
    ImGui::Text("Welcome to the best party in town!");
    ImGui::Separator();
    ImGui::PopFont();
    ImGui::PushFont(font2); // Set font1 as the default font for all ImGui elements
    ImGui::Dummy(ImVec2(0.0f, 15.0f));  // 50 pixels of vertical spacing

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("It's the year 2088 and sadly,").x) * 0.5f);
    ImGui::Text("It's the year 2088 and sadly,");
    ImGui::Separator();

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("you're the most awkward robot in the party").x) * 0.5f);
    ImGui::Text("you're the most awkward robot in the party");
    ImGui::Separator();

    ImGui::Dummy(ImVec2(0.0f, 25.0f));  // 50 pixels of vertical spacing

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Please make sure your keyboard is set to english").x) * 0.5f);
    ImGui::Text("Please make sure your keyboard is set to english");
    ImGui::Separator();

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("and you are not in caps lock!").x) * 0.5f);
    ImGui::Text("and you are not in caps lock!");


    if (debug_mode) {
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::PopFont();
    ImGui::PushFont(font3); // Set font1 as the default font for all ImGui elemen

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.65f);
    ImGui::Indent(ImGui::GetWindowWidth() * 0.32f);
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 128, 255));  // Navy blue border color
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(150, 150, 255, 255));  // Brighter blue hover color
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);  // Thickness of the border
    ImGui::Dummy(ImVec2(0.0f, 25.0f));  // 50 pixels of vertical spacing


    // Add the mute/unmute checkbox
    if (ImGui::Checkbox("Mute Music", &isMuted)) {
        if (isMuted) {
            Mix_VolumeMusic(0); // Mute the music
        }
        else {
            Mix_VolumeMusic(100); // Set the volume to the current level
        }
    }
    ImGui::PopFont();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

    // Add some space before the next control
    ImGui::Spacing();

    ImGui::PushFont(font3); // Set font1 as the default font for all ImGui elemen
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Full Screen").x) * 0.45f);
    // Set up colors and styles for the border and hover effect
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 128, 255));  // Navy blue border color
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(150, 150, 255, 255));  // Brighter blue hover color
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);  // Thickness of the border

    // Render the checkbox
    if (ImGui::Checkbox("Full Screen", &fullScreenChecked)) {
        toggleFullScreen();
    }

    // Restore the previous styles
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);

    ImGui::Dummy(ImVec2(0.0f, 15.0f));  // 50 pixels of vertical spacing

    // Calculate the desired button width as 30% of the window size
    float buttonWidth = ImGui::GetWindowSize().x * 0.3f;

    // Center the button by setting the cursor X position
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - buttonWidth) * 0.5f);

    if (ImGui::Button("EXIT", ImVec2(buttonWidth, 0.0f))) {
        ImGui::OpenPopup("Already leaving?");
    }

    ImGui::PopStyleColor(3);

    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Already leaving?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::PopFont();
        ImGui::PushFont(font2); // Set font2 as the default font for all ImGui elements

        ImGui::Text("Are you sure you want to leave the party ?\n\n");
        ImGui::Separator();

        // Calculate the width needed to center the buttons
        float buttonWidth = 120.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = (buttonWidth * 2.0f) + spacing;

        // Align the buttons to the center
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - totalWidth) * 0.5f);

        if (ImGui::Button("yes", ImVec2(buttonWidth, 0))) {
            glutLeaveMainLoop();
        }
        ImGui::SetItemDefaultFocus();

        ImGui::SameLine();

        if (ImGui::Button("no", ImVec2(buttonWidth, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopFont(); // Restore the previous font
    ImGui::Unindent(ImGui::GetWindowWidth() * 0.25f);

    ImGui::End();
    ImGui::PopFont(); // Restore the previous font


}

