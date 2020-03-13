#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "src/wrapedgl.h"


void glfwErrorCallback(int error, const char* description) {
  fprintf(stderr, "GLFW Error: %s\n", description);
}

GLFWwindow* initWindow() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow* window = glfwCreateWindow(1000, 1000, "MyWindow", NULL, NULL);

  if (window == NULL) {
    printf("Window creation failed\n");
    exit(1);
  }

  glfwSetWindowSizeLimits(window, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwMakeContextCurrent(window);

  return window;
}


void main() {
  glfwInit();

  GLFWwindow* window = initWindow();

	glfwSetErrorCallback(glfwErrorCallback);

  wglInit();
  WrappedGL* wgl = wglCreateContext();

  wglCreateProgram(wgl);

  wglLoadVertexShaderFromFile(wgl, "src/default.vert");
  wglLoadFragmentShaderFromFile(wgl, "src/display.frag");

  wglLinkProgram(wgl);
  wglUseProgram(wgl);
  // float vertices[] = {
  //   -1, -1, 0,  -1, 1, 0,  1, 1, 0,
  //   1, 1, 1,  1, -1, 0,  -1, -1, 0
  // };

  // wglInitVertexBuffer(wgl);
  wglInitVertexArray(wgl);

  // wglAddVertices(wgl, vertices);


  while (!glfwWindowShouldClose(window)) {
    wglRender(wgl);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}