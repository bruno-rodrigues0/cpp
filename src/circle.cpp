// Include headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

// Defines
#define WIN_WIDTH 600
#define WIN_HEIGHT 600

// Initializing functions
GLFWwindow *StartWindow();
GLuint CreateShaders(const char *vertexShaderSource, const char *fragmentShaderSource);
void CreateGLO(GLuint &VAO, GLuint &VBO, const glm::vec3 *vertices, size_t vertexCount);
void buildCircle(const float radius, const int vertexCount);
void process_input(GLFWwindow *window);

using namespace std;

// Shaders Sources
const char *vertexShaderSource { R"glsl(
  #version 460 core 
  layout (location = 0) in vec3 aPos;
  void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)glsl" };

const char *fragmentShaderSource { R"glsl(
  #version 460 core 
  out vec4 FragColor;
  void main(){
    FragColor = vec4(1.0f, 0.4f, 0.5f, 1.0f);
  } 
)glsl" };

vector<glm::vec3> vertices;

int main() { 
  cout << "Initializing..." << endl;

  GLFWwindow *window { StartWindow() };

  buildCircle(0.5f, 8);

  long unsigned int vertexCount { vertices.size() };

  GLuint VAO, VBO;

  GLuint shaderProgram { CreateShaders(vertexShaderSource, fragmentShaderSource) };
  CreateGLO(VAO, VBO, vertices.data(), vertexCount);

  while (!glfwWindowShouldClose(window)){
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO); 
  glDeleteProgram(shaderProgram);
}

GLFWwindow *StartWindow(){
  if(!glfwInit()){
    cerr << "Error creating window." << endl;
    glfwTerminate();
    return 0;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window { glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "~", NULL, NULL) };
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
      cerr << "Failed to initialize GLEW" << endl;
      return nullptr;
  }

  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  return window;
}

GLuint CreateShaders(const char *vertexShaderSource, const char *fragmentShaderSource){
  GLuint vertexShader   { glCreateShader(GL_VERTEX_SHADER) };
  GLuint fragmentShader { glCreateShader(GL_FRAGMENT_SHADER) };
  GLuint shaderProgram  { glCreateProgram() };
  GLint success;

  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char infoLog [512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    cerr << infoLog << endl;
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
    char infoLog [512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    cerr << infoLog << endl;
  }

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success){
    char infoLog [512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    cerr << infoLog << endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void CreateGLO(GLuint &VAO, GLuint &VBO, const glm::vec3 *vertices, size_t vertexCount){

  // VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

void buildCircle(const float radius, const int vertexCount){
  const float angle { 360.0f / vertexCount };

  const int triangleCount { vertexCount -2 };

  vector<glm::vec3> temp;

  for (int i { 0 }; i < vertexCount; i++){
    float currentAngle { i * angle };
    float x { radius * cos(glm::radians(currentAngle)) };
    float y { radius * sin(glm::radians(currentAngle)) };
    float z { 0.0f };


    temp.push_back(glm::vec3(x, y, z));
  }

  for (int i { 0 }; i < triangleCount; i++){
    vertices.push_back(temp[0]);
    vertices.push_back(temp[i + 1]);
    vertices.push_back(temp[i + 2]);
  } 
}

void process_input(GLFWwindow *window){
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

