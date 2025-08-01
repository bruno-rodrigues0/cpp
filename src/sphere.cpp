#include "utils/camera.h"
#include "utils/window.h"
#include "utils/shader.h"
#include "utils/Buffers.h"
#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

void buildSphere();

const char *vertexShaderSource = R"glsl(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main(){
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 460 core
    out vec4 FragColor;
    void main(){
        FragColor = vec4(1.0f, 0.3f, 0.3f, 1.0f);
    }
)glsl";

Camera *camera = new Camera();
std::vector<float> vertices {};
std::vector<float> normals {};

const float radius {1.0f};
const float PI { 3.1415 };

int main(int argc, char *argv[]) {
    GLFWwindow *window = StartWindow();
    GLuint shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);
    GLuint VAO, VBO;
    CreateVBOVAO(VAO, VBO, vertices.data(), vertices.size());
    buildSphere();

    while (!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model{glm::mat4(1.0f)};
        glm::mat4 view{glm::mat4(1.0f)};
        glm::mat4 projection{glm::mat4(1.0f)};

        view = camera->GetViewMatrix();
        projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 800.0f,
                                    0.1f, 100.0f);

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

glm::vec3 sphericalToCartesian(float r, float theta, float phi) {
  float x = r * sin(theta) * cos(phi);
  float y = r * cos(theta);
  float z = r * sin(theta) * sin(phi);
  return glm::vec3(x, y, z);
};

void buildSphere(){
    int stacks = 10;
    int sectors = 10;

    // generate circumference points using integer steps
    for (float i = 0.0f; i <= stacks; ++i) {
      float theta1 = (i / stacks) * glm::pi<float>();
      float theta2 = (i + 1) / stacks * glm::pi<float>();
      for (float j = 0.0f; j < sectors; ++j) {
        float phi1 = j / sectors * 2 * glm::pi<float>();
        float phi2 = (j + 1) / sectors * 2 * glm::pi<float>();
        glm::vec3 v1 = sphericalToCartesian(radius, theta1, phi1);
        glm::vec3 v2 = sphericalToCartesian(radius, theta1, phi2);
        glm::vec3 v3 = sphericalToCartesian(radius, theta2, phi1);
        glm::vec3 v4 = sphericalToCartesian(radius, theta2, phi2);

        // Triangle 1: v1-v2-v3
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z}); //      /|
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z}); //     / |
        vertices.insert(vertices.end(), {v3.x, v3.y, v3.z}); //    /__|

        // Triangle 2: v2-v4-v3
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z});
        vertices.insert(vertices.end(), {v4.x, v4.y, v4.z});
        vertices.insert(vertices.end(), {v3.x, v3.y, v3.z});
      }
    }
}


