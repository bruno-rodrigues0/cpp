#ifndef WINDOW_H
#define WINDOW_H

#include <string>

struct GLFWwindow;
class Window {
public:
  Window(char *title, unsigned int width, unsigned int heigth);

private:
  std::string Title;
  unsigned int Width;
  unsigned int Height;
  GLFWwindow *window;

  GLFWwindow *StartWindow();
};

#endif // !WINDOW_H
