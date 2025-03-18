#include "../include/camera.hpp"
#include "../include/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"



Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: front(glm::vec3(0.0f, 0.0f, 1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    if (direction == FORWARD) this->position += this->front * velocity;
    if (direction == BACKWARD) this->position -= this->front * velocity;
    if (direction == LEFT) this->position -= this->right * velocity;
    if (direction == RIGHT) this->position += this->right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw = glm::mod(this->yaw + xoffset, 360.0f);
    this->pitch += yoffset;

    // clamping
    if(constrainPitch) {
        if(this->pitch > 89.0f) this->pitch = 89.0f;
        if(this->pitch < -89.0f) this->pitch = -89.0f;
    }

    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    this->fov -= (float)yoffset;

    if(this->fov < 1.0f) this->fov = 1.0f;
    if(this->fov > 45.0f) this->fov = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;

    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}