#include "Robot.h"
#include <cmath> // Include for sin() function
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Robot::Robot()
    : posX(0), posY(1), posZ(0), directionAngle(0),
    headVerticalAngle(0), headHorizontalAngle(0),
    leftUpperArmAngle(0), leftLowerArmAngle(0), leftWristAngle(0),
    rightUpperArmAngle(0), rightLowerArmAngle(0), rightWristAngle(0),
    leftUpperLegAngle(0), leftLowerLegAngle(0), leftAnkleAngle(0),
    rightUpperLegAngle(0), rightLowerLegAngle(0), rightAnkleAngle(0),
    leftFootAngle(0), rightFootAngle(0),
    leftFootY(0), rightFootY(0),
    legWalkingAngle(0) {}

void Robot::draw() {
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    /*
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    */
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(directionAngle, 0.0, 1.0, 0.0);
    drawBody();
    drawLegs();
    drawArms();
    drawHead();
    glPopMatrix();
}

void Robot::rotateArmShoulder(float angle, bool left) {
    if (left) {
        leftUpperArmAngle += angle;
        if (leftUpperArmAngle < LEFT_UPPER_ARM_MIN) {
            leftUpperArmAngle = LEFT_UPPER_ARM_MIN;
        }
        else if (leftUpperArmAngle > LEFT_UPPER_ARM_MAX) {
            leftUpperArmAngle = LEFT_UPPER_ARM_MAX;
        }
    }
    else {
        rightUpperArmAngle += angle;
        if (rightUpperArmAngle < RIGHT_UPPER_ARM_MIN) {
            rightUpperArmAngle = RIGHT_UPPER_ARM_MIN;
        }
        else if (rightUpperArmAngle > RIGHT_UPPER_ARM_MAX) {
            rightUpperArmAngle = RIGHT_UPPER_ARM_MAX;
        }
    }
}

void Robot::rotateArmElbow(float angle, bool left) {
    if (left) {
        leftLowerArmAngle += angle;
        if (leftLowerArmAngle < LEFT_LOWER_ARM_MIN) {
            leftLowerArmAngle = LEFT_LOWER_ARM_MIN;
        }
        else if (leftLowerArmAngle > LEFT_LOWER_ARM_MAX) {
            leftLowerArmAngle = LEFT_LOWER_ARM_MAX;
        }
    }
    else {
        rightLowerArmAngle += angle;
        if (rightLowerArmAngle < RIGHT_LOWER_ARM_MIN) {
            rightLowerArmAngle = RIGHT_LOWER_ARM_MIN;
        }
        else if (rightLowerArmAngle > RIGHT_LOWER_ARM_MAX) {
            rightLowerArmAngle = RIGHT_LOWER_ARM_MAX;
        }
    }
}

void Robot::rotateArmWrist(float angle, bool left) {
    if (left) {
        leftWristAngle += angle;
        if (leftWristAngle < LEFT_WRIST_MIN) {
            leftWristAngle = LEFT_WRIST_MIN;
        }
        else if (leftWristAngle > LEFT_WRIST_MAX) {
            leftWristAngle = LEFT_WRIST_MAX;
        }
    }
    else {
        rightWristAngle += angle;
        if (rightWristAngle < RIGHT_WRIST_MIN) {
            rightWristAngle = RIGHT_WRIST_MIN;
        }
        else if (rightWristAngle > RIGHT_WRIST_MAX) {
            rightWristAngle = RIGHT_WRIST_MAX;
        }
    }
}

void Robot::rotateLeg(float angle, bool left) {
    if (left) {
        leftUpperLegAngle += angle;
        leftLowerLegAngle += angle;
        leftAnkleAngle += angle;
        leftFootY = sin(leftUpperLegAngle * M_PI / 180.0) * 2.5; // Adjust based on leg height
    }
    else {
        rightUpperLegAngle += angle;
        rightLowerLegAngle += angle;
        rightAnkleAngle += angle;
        rightFootY = sin(rightUpperLegAngle * M_PI / 180.0) * 2.5; // Adjust based on leg height
    }
}

void Robot::rotateFoot(float angle, bool left) {
    if (left) {
        leftFootAngle += angle;
    }
    else {
        rightFootAngle += angle;
    }
}

void Robot::moveLegs() {
   
    if ( walkingPhase == 0){
        rotateLeg(-4.0, true);
        rotateLeg(4.0, false);
        walkingPhase = 1;
    } 
    else {

        rotateLeg(4.0, true);
        rotateLeg(-4.0, false);
        walkingPhase =0;

    }

}

void Robot::rotateHead(float angleVertical, float angleHorizontal) {
    headVerticalAngle += angleVertical;
    headHorizontalAngle += angleHorizontal;
}


void Robot::setPosition(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
}

void Robot::setDirection(float angle) {
    directionAngle = angle;
}

void Robot::getView(float& eyeX, float& eyeY, float& eyeZ, float& centerX, float& centerY, float& centerZ) {
    eyeX = posX;
    eyeY = posY + 5.0f; // height of the robot's eyes
    eyeZ = posZ;

    // Normalize direction angle to be within 0 to 360 degrees
    if (getDirection() >= 360.0f) {
        setDirection(getDirection() -360);
    }
    else if (getDirection() < 0.0f) {
        setDirection(getDirection() + 360);
    }

    // Calculate the view direction vector
    float directionRadians = glm::radians(getDirection());
    glm::vec3 forward(cos(directionRadians), 0, sin(directionRadians));

    // Calculate the head rotation effects
    glm::vec3 viewVector = getViewVector();

    // Combine the forward direction with head rotation
    glm::vec3 combinedView = glm::normalize(forward + viewVector);

    centerX = eyeX + combinedView.x;
    centerY = eyeY + combinedView.y;
    centerZ = eyeZ + combinedView.z;
}





float Robot::getPositionX() const {
    return posX;
}

float Robot::getPositionY() const {
    return posY;
}

float Robot::getPositionZ() const {
    return posZ;
}

float Robot::getDirection() const {
    return directionAngle;
}

void Robot::drawBody() {
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
    GLfloat mat_shininess[] = { 76.8f }; // High value for a shiny effect

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    float bodyWidth = 1.5, bodyHeight = 3.0, bodyDepth = 0.8;
    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 2.5); // adjust position
    glScalef(bodyWidth, bodyDepth, bodyHeight);
    glutSolidCube(1.0);
    glPopMatrix();
}

void Robot::drawLegs() {
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
    GLfloat mat_shininess[] = { 76.8f }; // High value for a shiny effect

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    float bodyWidth = 1.5, bodyHeight = 3.0, legWidth = bodyWidth * 0.3, legHeight = 1.5, legDepth = 0.4;
    float footWidth = legWidth, footHeight = legHeight * 0.2, footDepth = legDepth * 1.5;

    GLUquadric* quadric = gluNewQuadric();

    // Right leg
    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    glTranslatef(bodyWidth * 0.25, rightFootY, 2.5 - (bodyHeight / 2) - 0.7);
    glRotatef(rightUpperLegAngle, -1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(legWidth, legDepth, legHeight);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right foot
    glTranslatef(0.0, 0.0, -legHeight / 2 - footHeight / 2); // Move to the bottom of the leg
    glRotatef(rightFootAngle, -1.0, 0.0, 0.0); // Apply foot rotation
    glScalef(footWidth, footHeight*2, footDepth*0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Left leg
    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    glTranslatef(-(bodyWidth * 0.25), leftFootY, 2.5 - (bodyHeight / 2) - 0.7);
    glRotatef(leftUpperLegAngle, -1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(legWidth, legDepth, legHeight);
    glutSolidCube(1.0);
    glPopMatrix();

    // Left foot
    glTranslatef(0.0, 0.0, -legHeight / 2 - footHeight / 2); // Move to the bottom of the leg
    glRotatef(leftFootAngle, -1.0, 0.0, 0.0); // Apply foot rotation
    glScalef(footWidth, footHeight*2, footDepth * 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}

void Robot::drawArms() {
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
    GLfloat mat_shininess[] = { 76.8f }; // High value for a shiny effect

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLUquadric* quadric = gluNewQuadric();
    float bodyWidth = 1.5, bodyHeight = 3.0, upperRadius = 0.2, upperArmHeight = 1.2, lowerArmRadius = upperRadius * 0.8, lowerArmHeight = upperArmHeight;
    float wristWidth = (lowerArmRadius * 2) * 1.2, wristHeight = lowerArmHeight * 0.35, wristDepth = lowerArmRadius;
    float fingerRadius = lowerArmRadius * 0.4, fingerHeight = wristHeight, thumbProportionalToWrist = 0.6;

    // right arm
    glPushMatrix();
    // upper arm
    glRotatef(270, 1.0f, 0.0f, 0.0f);

    glTranslatef(bodyWidth / 2 + upperRadius, 0.0, 2.5 - (bodyHeight / 2) + bodyHeight * 0.95);
    glRotatef(rightUpperArmAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -upperArmHeight);
    gluCylinder(quadric, upperRadius, upperRadius, upperArmHeight, 20, 1);
    // lower arm
    glRotatef(rightLowerArmAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -lowerArmHeight);
    gluCylinder(quadric, lowerArmRadius, lowerArmRadius, lowerArmHeight, 20, 1);
    // wrist
    glRotatef(rightWristAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -(wristHeight / 2));
    glPushMatrix();
    glScalef(wristWidth, wristDepth, wristHeight);
    glutSolidCube(1.0);
    glPopMatrix();
    // fingers
    glTranslatef(wristWidth / 2 - fingerRadius, 0.0, -(wristHeight / 2) - fingerHeight);
    for (int i = 0; i < 4; i++) {
        gluCylinder(quadric, fingerRadius, fingerRadius, fingerHeight, 20, 1);
        glTranslatef(-(wristWidth / 4), 0.0, 0.0);
    }
    glPopMatrix();

    // left arm
    glPushMatrix();
    glRotatef(270, 1.0f, 0.0f, 0.0f);

    // upper arm
    glTranslatef(-(bodyWidth / 2 + upperRadius), 0.0, 2.5 - (bodyHeight / 2) + bodyHeight * 0.95);
    glRotatef(leftUpperArmAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -upperArmHeight);
    gluCylinder(quadric, upperRadius, upperRadius, upperArmHeight, 20, 1);
    // lower arm
    glRotatef(leftLowerArmAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -lowerArmHeight);
    gluCylinder(quadric, lowerArmRadius, lowerArmRadius, lowerArmHeight, 20, 1);
    // wrist
    glRotatef(leftWristAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, -(wristHeight / 2));
    glPushMatrix();
    glScalef(wristWidth, wristDepth, wristHeight);
    glutSolidCube(1.0);
    glPopMatrix();
    // fingers
    glTranslatef(-(wristWidth / 2 - fingerRadius), 0.0, -(wristHeight / 2) - fingerHeight);
    for (int i = 0; i < 4; i++) {
        gluCylinder(quadric, fingerRadius, fingerRadius, fingerHeight, 20, 1);
        glTranslatef((wristWidth / 4), 0.0, 0.0);
    }
    glPopMatrix();
}

glm::vec3 Robot::getViewPos() {
    // Get the robot's position
    float posX = getPositionX();
    float posY = getPositionY();
    float posZ = getPositionZ();

    // Set the eye position based on the robot's position and head height
    float eyeX = posX;
    float eyeY = posY + 5.0f; // Height of the robot's eyes
    float eyeZ = posZ;

    // Calculate the view direction based on the robot's direction and head angles
    glm::vec3 view_vector = getViewVector();

    // Set the center position based on the view vector
    float centerX = eyeX + view_vector.x;
    float centerY = eyeY + view_vector.y;
    float centerZ = eyeZ + view_vector.z;

    return glm::vec3(centerX, centerY, centerZ);
}



glm::vec3 Robot::getViewTarget() {
    glm::vec3 view_vector = getViewVector();
    glm::vec3 eyes = getViewPos();
    return eyes + view_vector; // target - eyes pos = view vector => target = view vector + eyes pos
}

glm::vec3 Robot::getViewVector() {
    // Get the robot's current direction and convert it to radians
    float direction = getDirection();
    float rad_angle = glm::radians(direction);

    // Calculate the forward direction based on robot's direction
    glm::vec3 forward(cos(rad_angle), 0, sin(rad_angle));

    // Apply vertical head rotation
    float rad_angle_vert = glm::radians(headVerticalAngle);
    glm::mat4 rotationMatVert = glm::rotate(glm::mat4(1.0f), rad_angle_vert, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 view_vector = glm::vec3(rotationMatVert * glm::vec4(forward, 1.0f));

    // Apply horizontal head rotation
    float rad_angle_hor = glm::radians(headHorizontalAngle);
    glm::mat4 rotationMatHor = glm::rotate(glm::mat4(1.0f), rad_angle_hor, glm::vec3(0.0f, 1.0f, 0.0f));
    view_vector = glm::vec3(rotationMatHor * glm::vec4(view_vector, 1.0f));

    return glm::normalize(view_vector);
}



void Robot::drawHead() {
    GLfloat whiteAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat whiteDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat whiteSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat whiteShininess[] = { 50.0f };

    GLfloat blueAmbient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat blueDiffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat blueSpecular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat blueShininess[] = { 50.0f };

    GLfloat blackAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat blackDiffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat blackSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat blackShininess[] = { 50.0f };

    GLUquadric* quadric = gluNewQuadric();
    float headWidth = 0.6, headHeight = 0.7, headDepth = 0.35;
    float eyesRadius = 0.1, eyesDistanceProportionalToHead = 0.45, eyesHeightProportionalToHead = 0.7, pupilsRadiusProportionalToEyes = 0.5;
    float mouthWidth = 0.5, mouthHeight = 0.00001;

    // neck & head
    glPushMatrix();
    glRotatef(270, 1.0f, 0.0f, 0.0f);

    glTranslatef(0.0, 0.0, 2.5 + 3.0 / 2);
    gluCylinder(quadric, 0.2, 0.2, 0.35, 20, 1);
    glTranslatef(0.0, 0.0, 0.35);
    glRotatef(headHorizontalAngle, 0.0f, 0.0f, -1.0f);
    glRotatef(headVerticalAngle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0, 0.0, headHeight / 2);
    glPushMatrix();
    glScalef(headWidth, headDepth, headHeight);
    glutSolidCube(1.0);
    glPopMatrix();

    // eyes
    glPushMatrix();

    // Left eye
    glTranslatef((headWidth / 2) * eyesDistanceProportionalToHead, -(headDepth / 2), -(headHeight / 2) + eyesHeightProportionalToHead * headHeight);

    // Set color for the white part of the eye
    glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, whiteShininess);
    glColor3f(1.0, 1.0, 1.0); // Ensure the color is set to white
    glutSolidSphere(eyesRadius, 20, 20);

    // Set color for the blue pupil
    glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blueSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, blueShininess);
    glColor3f(0.0, 0.0, 1.0); // Ensure the color is set to blue
    glTranslatef(0.0, -eyesRadius * 0.6, 0.0);
    glutSolidSphere(pupilsRadiusProportionalToEyes * eyesRadius, 20, 20);
    glPopMatrix();

    glPushMatrix();

    // Right eye
    glTranslatef(-(headWidth / 2) * eyesDistanceProportionalToHead, -(headDepth / 2), -(headHeight / 2) + eyesHeightProportionalToHead * headHeight);

    // Set color for the white part of the eye
    glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, whiteShininess);
    glColor3f(1.0, 1.0, 1.0); // Ensure the color is set to white
    glutSolidSphere(eyesRadius, 20, 20);

    // Set color for the blue pupil
    glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blueSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, blueShininess);
    glColor3f(0.0, 0.0, 1.0); // Ensure the color is set to blue
    glTranslatef(0.0, -eyesRadius * 0.6, 0.0);
    glutSolidSphere(pupilsRadiusProportionalToEyes * eyesRadius, 20, 20);
    glPopMatrix();

    // mouth
    glPushMatrix();
    glTranslatef(0.0, -(headDepth / 2), -(headHeight / 2) + headHeight * 0.3);

    // Set color for the mouth
    glMaterialfv(GL_FRONT, GL_AMBIENT, blackAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blackDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blackSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, blackShininess);
    glColor3f(0.0, 0.0, 0.0); // Ensure the color is set to black

    glScalef(mouthWidth, mouthHeight, 0.07);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

