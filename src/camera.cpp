#include "utils/camera.h"
#include "utils/window.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint CreateShaderProgram(const char *vertexSource,
                           const char *fragmentSource);
void CreateVBOVAO(GLuint &VAO, GLuint &VBO, GLuint &EBO, const float *vertices,
                  const int *indices, size_t vertexCount, size_t indexCount);
void CreateTexture(unsigned int &texture, const char *image_path);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const char *vertexShaderSource = R"glsl(
  #version 460 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec2 aTexCoord;

  out vec2 TexCoord;

  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;

  void main()
  {
      gl_Position = projection * view * model * vec4(aPos, 1.0);
      TexCoord = vec2(aTexCoord.x, aTexCoord.y);
  }
)glsl";

const char *fragShaderSource = R"glsl(
  #version 460 core
  out vec4 FragColor;
    
  in vec2 TexCoord;

  uniform sampler2D ourTexture;

  void main()
  {
      FragColor = texture(ourTexture, TexCoord);
  }
)glsl";

Camera *camera = new Camera();

float deltaTime{0.0f}, lastFrame{0.0f};
float lastX{400}, lastY{400};
bool firstMouse;

int main() {
  GLFWwindow *window = StartWindow();
  GLuint shaderProgram =
      CreateShaderProgram(vertexShaderSource, fragShaderSource);

  std::vector<float> vertices = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  std::vector<int> indices = {0, 1, 3, 1, 2, 3};

  // Texture
  unsigned int texture;
  CreateTexture(texture, "cont.png");

  int vertexCount = vertices.size();
  int indexCount = indices.size();

  GLuint VBO, VAO, EBO;
  CreateVBOVAO(VAO, VBO, EBO, vertices.data(), indices.data(), vertexCount,
               indexCount);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glClearColor(0.1f, 0.1f, 0.2f, 0.7f);
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

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    for (int i{0}; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * (i + 1);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle),
                          glm::vec3(1.0f, 0.3f, 0.5f));
      int modelLoc = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void process_input(GLFWwindow *window) {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->ProcessKeyboard(RIGHT, deltaTime);
  }
}

void CreateVBOVAO(GLuint &VAO, GLuint &VBO, GLuint &EBO, const float *vertices,
                  const int *indices, size_t vertexCount, size_t indexCount) {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices,
               GL_STATIC_DRAW);

  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indices,
  //              GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

GLuint CreateShaderProgram(const char *vertexSource,
                           const char *fragmentSource) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  GLuint shaderProgram = glCreateProgram();
  GLint success;

  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // CORRETO
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
  }

  glShaderSource(fragShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragShader);

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success); // CORRETO
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
    std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
  }

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed: " << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);

  return shaderProgram;
}

void CreateTexture(unsigned int &texture, const char *image_path) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);

  int width, height, nrChannels;
  unsigned char *data{stbi_load(image_path, &width, &height, &nrChannels, 0)};

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  camera->ProcessMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera->ProcessMouseScroll((float)yoffset);
}
