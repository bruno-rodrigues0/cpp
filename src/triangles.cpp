#include "utils/window.h"
#include "utils/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

void CreateGLO(GLuint &VAO, GLuint &VBO, GLfloat *vertices, int vertexCount);
void process_input(GLFWwindow *window);
GLuint CreateShader(const char *vertexShaderSource, const char *fragmentShaderSource);

const char *vertexShaderSource = R"glsl(
    #version 460 core
    layout (location = 0) in vec3 vertices;
    void main(){
        gl_Position = vec4(vertices, 1.0);
    }
)glsl";

const char *fragmentShaderSource = R"glsl(
    #version 460 core
    out vec4 FragColor;
    void main(){
        FragColor = vec4(1.0f, 0.4f, 0.5f, 1.0f);
    }
)glsl";

int main(int argc, char *argv[]) {
    // Create a window
    GLFWwindow *window = StartWindow();

    // Define the shape vertices
    std::vector<GLfloat> vertices1 {
        -0.5f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };
    std::vector<GLfloat> vertices2 {
        0.5f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };
    std::vector<GLfloat> vertices3 {
        0.5f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Buffers
    GLuint VAO, VBO;

    GLuint shaderProgram = CreateShader(vertexShaderSource, fragmentShaderSource);

    // Render loop
    while(!glfwWindowShouldClose(window)){
        process_input(window);
        glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        CreateGLO(VAO, VBO, vertices1.data(), vertices1.size());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,  vertices1.size() / 3);
        glBindVertexArray(0);

        CreateGLO(VAO, VBO, vertices2.data(), vertices2.size());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,  vertices2.size() / 3);
        glBindVertexArray(0);

        CreateGLO(VAO, VBO, vertices3.data(), vertices3.size());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,  vertices3.size() / 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void CreateGLO(GLuint &VAO, GLuint &VBO, GLfloat *vertices, int vertexCount){
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
}

void process_input(GLFWwindow *window){
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
