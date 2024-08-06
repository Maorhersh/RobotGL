#ifndef ROBOT_H
#define ROBOT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>
// Define angle limits
// Define more realistic angle limits (in degrees)
const float LEFT_UPPER_ARM_MIN = -60.0f; // Shoulder joint: more realistic min angle
const float LEFT_UPPER_ARM_MAX = 60.0f;  // Shoulder joint: more realistic max angle
const float RIGHT_UPPER_ARM_MIN = -60.0f; // Shoulder joint: more realistic min angle
const float RIGHT_UPPER_ARM_MAX = 60.0f;  // Shoulder joint: more realistic max angle

const float LEFT_LOWER_ARM_MIN = 0.0f;   // Elbow joint: realistic min angle (fully extended)
const float LEFT_LOWER_ARM_MAX = 150.0f; // Elbow joint: realistic max angle (bent)
const float RIGHT_LOWER_ARM_MIN = 0.0f;  // Elbow joint: realistic min angle (fully extended)
const float RIGHT_LOWER_ARM_MAX = 150.0f; // Elbow joint: realistic max angle (bent)

const float LEFT_WRIST_MIN = -30.0f; // Wrist joint: realistic min angle
const float LEFT_WRIST_MAX = 30.0f;  // Wrist joint: realistic max angle
const float RIGHT_WRIST_MIN = -30.0f; // Wrist joint: realistic min angle
const float RIGHT_WRIST_MAX = 30.0f;  // Wrist joint: realistic max angle
class Robot {
public:
    Robot();

    void draw();
    void rotateArmShoulder(float angle, bool left);
    void rotateArmElbow(float angle, bool left);
    void rotateArmWrist(float angle, bool left);
    void rotateLeg(float angle, bool left); // Updated function for leg rotation
    void rotateFoot(float angle, bool left); // Function for foot rotation
    void moveLegs(); // Function for moving legs
    void rotateHead(float angleVertical, float angleHorizontal);
    void walkLegs(float angle);
    void setPosition(float x, float y, float z);
    void setDirection(float angle);
    void getView(float& eyeX, float& eyeY, float& eyeZ, float& centerX, float& centerY, float& centerZ);
    glm::vec3 getViewPos() ;
    glm::vec3 getViewTarget();
    glm::vec3 getViewVector();
    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;
    float getDirection() const;

private:
    void drawBody();
    void drawLegs();
    void drawArms();
    void drawHead();


    float centerX;
    float centerY;
    float centerZ;
    float eyeX, eyeY, eyeZ;
    float posX, posY, posZ;
    float directionAngle;
    float headVerticalAngle, headHorizontalAngle;
    float leftUpperArmAngle, leftLowerArmAngle, leftWristAngle;
    float rightUpperArmAngle, rightLowerArmAngle, rightWristAngle;
    float leftUpperLegAngle, leftLowerLegAngle, leftAnkleAngle;
    float rightUpperLegAngle, rightLowerLegAngle, rightAnkleAngle;
    float leftFootAngle, rightFootAngle; // Variables for foot angles
    float leftFootY, rightFootY; // Variables for foot positions
    float legWalkingAngle;
    float walkingPhase=0;


};

#endif
