#include "utils/window.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

Window window = new Window();
int main(int argc, char *argv[]) {
  GLFWwindow *window{};

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
  }
  return 0;
}
