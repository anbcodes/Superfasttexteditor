#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "src/wrapedgl.h"
#include <lodepng.h>

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

WrappedGL* initGL() {
  WrappedGL* wgl = wglCreateContext();

  wglCreateProgram(wgl);

  wglLoadVertexShaderFromFile(wgl, "src/default.vert");
  wglLoadFragmentShaderFromFile(wgl, "src/display.frag");

  wglLinkProgram(wgl);
  wglUseProgram(wgl);

  wglInitVertexArray(wgl);

  // // WrappedGLTexture* dataT = malloc(sizeof(WrappedGLTexture));

  // // GLfloat data[] = {
  // //   1, 0, 0, 0,  1, 0, 0, 0
  // // };

  // // dataT->type = GL_TEXTURE_1D;

  // // dataT->data = data;
  
  // // dataT->index = 0;

  // // wglInitTexture(wgl, dataT, "data");

  // // wgl->data = dataT;

  WrappedGLTexture* font = malloc(sizeof(WrappedGLTexture));

  font->type = GL_TEXTURE_2D;

  unsigned error;
  unsigned char* image = 0;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, "./fonts/RobotoMono-Regular.png");
  if (error) {
    printf("error %u: %s\n", error, lodepng_error_text(error));
  }

  wglInitTexture(wgl, font, "font");

  wglGenTexture2D(font, width, height, image);

  free(image);

  wgl->font = font;

  return wgl;
}

void main() {
  glfwInit();

  GLFWwindow* window = initWindow();

	glfwSetErrorCallback(glfwErrorCallback);

  wglInit();
  WrappedGL* wgl = initGL();


  while (!glfwWindowShouldClose(window)) {
    wglRender(wgl);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}