#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow *StartWindow() {
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW, panic" << std::endl;
    return nullptr;
  }
  GLFWwindow *window = glfwCreateWindow(800, 800, "~", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create a GLFW window." << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return window;
}
