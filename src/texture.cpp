#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

GLFWwindow *StartGLU();
GLuint CreateShaderProgram(const char *vertexSource,
                           const char *fragmentSource);
void CreateVBOVAO(GLuint &VAO, GLuint &VBO, GLuint &EBO, const float *vertices,
                  const int *indices, size_t vertexCount, size_t indexCount);
void CreateTexture(unsigned int texture, const char *image_path);
void process_input(GLFWwindow *window);

const char *vertexShaderSource = R"glsl(
  #version 460 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;
  layout (location = 2) in vec2 aTexCoord;

  out vec3 ourColor;
  out vec2 TexCoord;

  void main()
  {
      gl_Position = vec4(aPos, 1.0);
      ourColor = aColor;
      TexCoord = aTexCoord;
  }
)glsl";

const char *fragShaderSource = R"glsl(
  #version 460 core
  out vec4 FragColor;
    
  in vec3 ourColor;
  in vec2 TexCoord;

  uniform sampler2D ourTexture;

  void main()
  {
      FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
  }
)glsl";

int main() {
  GLFWwindow *window = StartGLU();
  GLuint shaderProgram =
      CreateShaderProgram(vertexShaderSource, fragShaderSource);

  vector<float> vertices = {
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  vector<int> indices = {0, 1, 3, 1, 2, 3};

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

    glClearColor(0.1f, 0.1f, 0.2f, 0.7f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

GLFWwindow *StartGLU() {
  if (!glfwInit()) {
    cout << "Failed to initialize GLFW, panic" << endl;
    return nullptr;
  }
  GLFWwindow *window = glfwCreateWindow(1366, 720, "~", NULL, NULL);
  if (!window) {
    cerr << "Failed to create a GLFW window." << endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW" << endl;
    glfwTerminate();
    return nullptr;
  }

  return window;
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
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

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indices,
               GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
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
    cerr << "Vertex shader compilation failed: " << infoLog << endl;
  }

  glShaderSource(fragShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragShader);

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success); // CORRETO
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
    cerr << "Fragment shader compilation failed: " << infoLog << endl;
  }

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    cerr << "Shader program linking failed: " << infoLog << endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);

  return shaderProgram;
}

void CreateTexture(unsigned int &texture, const char *image_path){
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);

  int width, height, nrChannels;
  unsigned char *data{
          stbi_load(image_path, &width, &height, &nrChannels, 0)};

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
