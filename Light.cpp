#include "Light.h"

// Constructor for the Light class
Light::Light(int id, GLfloat PosX, GLfloat PosY, GLfloat PosZ, string object, GLfloat scale,
    GLfloat cutoff, GLfloat exponent,
    GLfloat TargetX, GLfloat TargetY, GLfloat TargetZ) {
    // Initialize light properties
    this->id = id;
    this->position[0] = PosX;
    this->position[1] = PosY;
    this->position[2] = PosZ;
    this->position[3] = 1.0f;
    this->target[0] = TargetX;
    this->target[1] = TargetY;
    this->target[2] = TargetZ;
    this->cutoff = cutoff;
    this->exponent = exponent;

    // If an object file is provided, load it
    if (object.length() > 0) {
        this->object = new ObjectGL(object); // Load the 3D object
        this->object->scale = scale; // Scale the 3D object
    }

    // Enable the light
    enable();
    generateFlickerColors(); // Generate flicker colors
}

// Method to draw the light and its object (if any)
void Light::draw() {
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]); // Translate to the light's position
    fixDirection(); // Adjust the direction of the light

    if (this->object != NULL) {
        this->object->draw(); // Draw the 3D object representing the light
    }
    else {
        // Draw a default light representation
        GLfloat ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Increased brightness
        GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Increased brightness
        GLfloat shininess = 40.0f;

        // Set material properties for the light representation
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        // Draw the light representation (a cone and a cylinder)
        glutSolidCone(0.6, 0.9, 10, 10);
        glPushMatrix();
        glTranslatef(0, 0, 0.1f);
        glutSolidCylinder(0.3, 0.4, 10, 10);
        glPopMatrix();

        // Draw a sphere to represent the light source
        glDisable(GL_LIGHTING);
        glColor3fv(color);
        glutSolidSphere(0.3, 100, 100);
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

// Method to disable the light
void Light::disable() {
    glDisable(id);
}

void Light::enableFlicker() {
    flicker = true;
}

void Light::disableFlicker() {
    flicker = false;
}

// Method to enable the light
void Light::enable() {
    glEnable(id);
}

// Method to draw the light and its object (if any)


// Method to add the light to the scene
void Light::addlight() {
    // Add light only if it is enabled
    if (!glIsEnabled(id))
        return;

    // Set light properties
    glLightfv(id, GL_DIFFUSE, this->color); // Ensure 'color' is bright
    glLightfv(id, GL_SPECULAR, this->color); // Ensure 'color' is bright
    glLightfv(id, GL_POSITION, this->position);

    // Calculate and set the light direction
    GLfloat direction[3] = { this->target[0] - this->position[0],
                             this->target[1] - this->position[1],
                             this->target[2] - this->position[2] };
    glLightfv(this->id, GL_SPOT_DIRECTION, direction);
    glLightf(this->id, GL_SPOT_CUTOFF, this->cutoff);
    glLightf(this->id, GL_SPOT_EXPONENT, this->exponent);

    // Update flicker effect if enabled
    if (flicker) {
        updateFlicker();
    }
}


// Method to fix the direction of the light
void Light::fixDirection() {
    glm::vec3 eye = glm::vec3(this->position[0], this->position[1], this->position[2]); // The light's position
    glm::vec3 center = glm::vec3(this->target[0], this->target[1], this->target[2]); // The light's target

    // Calculate the desired direction vector
    glm::vec3 wantedVector = glm::normalize(center - eye);

    // Calculate the normal vector to the plane containing the current vector and the new vector
    glm::vec3 normal = glm::cross(towardVector, wantedVector);
    normal = glm::normalize(normal);

    // Calculate the angle between the current direction and the desired direction
    float radian_angle = glm::angle(towardVector, wantedVector);
    float angle = radian_angle * 180 / glm::pi<float>();

    // Rotate the drawing to align the light direction
    glRotatef(angle, normal.x, normal.y, normal.z);
}

// Method to generate random colors for the flicker effect
void Light::generateFlickerColors() {
    for (int i = 0; i < 10; ++i) {
        flickerColors.push_back(glm::vec3(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX
        ));
    }
}

// Method to update the light color for the flicker effect
void Light::updateFlicker() {
    flickerIndex = (flickerIndex + 1) % flickerColors.size();
    glm::vec3 newColor = flickerColors[flickerIndex];
    this->color[0] = newColor.r;
    this->color[1] = newColor.g;
    this->color[2] = newColor.b;
}
