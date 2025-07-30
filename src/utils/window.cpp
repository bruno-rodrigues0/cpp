#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Window::Window(char *title, unsigned int width, unsigned int height)
    : Title(title), Width(width), Height(height) {
  window = StartWindow();
}

GLFWwindow *Window::StartWindow() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
  }

  GLFWwindow *window =
      glfwCreateWindow(Width, Height, Title.data(), NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create a window" << std::endl;
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
