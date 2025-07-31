#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovements { FORWARD, BACKWARD, LEFT, RIGHT };

const float YAW{-90.0f};
const float PITCH{0.0f};
const float SPEED{2.5f};
const float SENSITIVITY{0.1f};
const float ZOOM{45.0f};

class Camera {
public:
  glm::vec3 Front;
  glm::vec3 Position;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;

  float MovSpeed;
  float MouseSensitivity;
  float Zoom;

  float lastX = 400, lastY = 400;
  bool firstMouse = true;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Position(position), WorldUp(up),
        Yaw(yaw), Pitch(pitch), MovSpeed(SPEED), MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void ProcessKeyboard(CameraMovements mov, float deltaTime) {
    float velocity = MovSpeed * deltaTime;

    if (mov == FORWARD)
      Position += Front * velocity;
    if (mov == BACKWARD)
      Position -= Front * velocity;
    if (mov == RIGHT)
      Position += Right * velocity;
    if (mov == LEFT)
      Position -= Right * velocity;
  }

  void ProcessMouseMovement(float xpos, float ypos,
                            GLboolean constrainPitch = true) {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
      if (Pitch < -89.0f) {
        Pitch = -89.0f;
      }
      if (Pitch > 89.0f) {
        Pitch = 89.0f;
      }
    }

    updateCameraVectors();
  }

  void ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) {
      Zoom = 1.0f;
    }
    if (Zoom > 45.0f) {
      Zoom = 45.0f;
    }
  }

private:
  void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

#endif // !CAMERA_H
