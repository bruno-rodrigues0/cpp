#include "utils/camera.h"
#include "utils/window.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f);

int main(int argc, char *argv[]) {
  GLFWwindow *raw = StartWindow();

  while (!glfwWindowShouldClose(raw)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
  }
  return 0;
}
