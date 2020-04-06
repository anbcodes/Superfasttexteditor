
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "wrapedgl.h"
#if defined(PRODUCTION)
#define checkForGLError(message)
#define logCompileErrors(shader)
#else
#define checkForGLError(message) optionalCheckForGLErrors(message)
#define logCompileErrors(shader) optionalLogCompileErrors(shader)
#endif

void wglInit() {
  if (!gladLoadGL()) {
    printf("Glad failed to load opengl\n");
    exit(-1);
  }
}


WrappedGL * wglCreateContext() {
  WrappedGL* wgl = malloc(sizeof(WrappedGL));
  return wgl;
}

void optionalCheckForGLErrors(char * message) {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR) {
    char *errStr;
    switch(err) {
      case GL_INVALID_OPERATION:      errStr="INVALID_OPERATION";      break;
      case GL_INVALID_ENUM:           errStr="INVALID_ENUM";           break;
      case GL_INVALID_VALUE:          errStr="INVALID_VALUE";          break;
      case GL_OUT_OF_MEMORY:          errStr="OUT_OF_MEMORY";          break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  errStr="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
    printf("GLERROR: %s (%d) while '%s'\n", errStr, err, message);
  }  
}

char * readFile(const char * path) {
  FILE * file = fopen(path, "r");
  char * buffer = 0;
	long length;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = malloc(length);
	if (buffer) {
		fread(buffer, 1, length, file);
	}
	fclose(file);

	buffer[length] = 0;
	
	return buffer;
}

void optionalLogCompileErrors(GLuint shader) {
  char infoLog[512];
  int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("Shader failed to compile: \n%s\n", infoLog);
  }
}


void wglCreateProgram(WrappedGL *wgl) {
  wgl->program = glCreateProgram();
  checkForGLError("Creating program");
}

void wglLinkProgram(WrappedGL *wgl) {
  glLinkProgram(wgl->program);
  checkForGLError("Linking program");
}

void wglUseProgram(WrappedGL* wgl) {
  glUseProgram(wgl->program);
  checkForGLError("Using program");
}

void wglLoadVertexShaderFromFile(WrappedGL *wgl, const char *path) {
  const char *vertexShaderSource = readFile(path);
  printf("Vertex Shader:\n%s\n", vertexShaderSource);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  checkForGLError("Creating vertex shader");
  glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
  checkForGLError("Adding source to vertex shader");
  glCompileShader(vertexShader);
  checkForGLError("Compiling vertex shader");
  logCompileErrors(vertexShader);
  glAttachShader(wgl->program, vertexShader);
  checkForGLError("Attaching vertex shader");
  glDeleteShader(vertexShader);
  checkForGLError("Deleting vertex shader");  
}

void wglLoadFragmentShaderFromFile(WrappedGL *wgl, const char *path) {
  const char *fragmentShaderSource = readFile(path);
  printf("fragment Shader:\n%s\n", fragmentShaderSource);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  checkForGLError("Creating fragment shader");
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
  checkForGLError("Adding source to fragment shader");
  glCompileShader(fragmentShader);
  checkForGLError("Compiling fragment shader");
  logCompileErrors(fragmentShader);
  glAttachShader(wgl->program, fragmentShader);
  checkForGLError("Attaching fragment shader");
  glDeleteShader(fragmentShader);
  checkForGLError("Deleting fragment shader");
}

void wglInitVertexArray(WrappedGL *wgl) {
  glGenVertexArrays(1, &wgl->vertexArrayObject);
  checkForGLError("Initializing vertex array");  
}

void wglInitTexture(WrappedGL *wgl, WrappedGLTexture* texture, const GLchar * name) {
  glUseProgram(wgl->program);
  checkForGLError("Using program (Init Texture)");
  glGenTextures(1, &texture->id);
  checkForGLError("Generating texture");
  glBindTexture(texture->type, texture->id);
  checkForGLError("Binding texture (Init)");
  glTexParameteri(texture->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  checkForGLError("Setting MIN_FILTER parameter");
  glTexParameteri(texture->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  checkForGLError("Setting MAG_FILTER parameter");
  texture->location = glGetUniformLocation(wgl->program, name);
  checkForGLError("Getting uniform location for texture");
  glUniform1i(texture->location, texture->index);
  checkForGLError("Setting uniform value for texture");
}

void wglGenTexture2D(WrappedGLTexture* texture, GLsizei width, GLsizei height, GLubyte * data) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  checkForGLError("Setting GL_PACK_ALIGNMENT");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
  checkForGLError("Generating 2d texture");
}

void wglGenTexture1D(WrappedGLTexture* texture, GLsizei width, GLubyte * data) {
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, width, 0, GL_RG, GL_UNSIGNED_BYTE, data);
  checkForGLError("Generating 1d texture");
}


void wglInitUniformVec2(WrappedGL *wgl, WGLUniformVec2* uniform) {
  glUseProgram(wgl->program);
  checkForGLError("Using program (Init Uniform)");
  uniform->location = glGetUniformLocation(wgl->program, uniform->name);
  checkForGLError("Getting uniform location");
  glUniform2f(uniform->location, uniform->value[0], uniform->value[1]);
  checkForGLError("Setting uniform value");
}

void wglInitUniformInt(WrappedGL *wgl, WGLUniformInt* uniform) {
  glUseProgram(wgl->program);
  checkForGLError("Using program (Init Uniform)");
  uniform->location = glGetUniformLocation(wgl->program, uniform->name);
  checkForGLError("Getting uniform location");
  glUniform1i(uniform->location, uniform->value);
  checkForGLError("Setting uniform value");
}

void wglUpdateUniforms(WrappedGL *wgl) {
  glUniform1i(wgl->mouseButton->location, wgl->mouseButton->value);
  checkForGLError("Setting uniform value");
  glUniform1i(wgl->mouseDown->location, wgl->mouseDown->value);
  checkForGLError("Setting uniform value");
  glUniform2f(wgl->mouse->location, wgl->mouse->value[0], wgl->mouse->value[1]);
  checkForGLError("Setting uniform value");
}

void wglRender(WrappedGL *wgl) {
  glClear(GL_COLOR_BUFFER_BIT);
  checkForGLError("Clearing");

  // glActiveTexture(GL_TEXTURE0 + wgl->data->index);
  // checkForGLError("Activating Texture");
  // glBindTexture(wgl->data->type, wgl->data->id);
  // checkForGLError("Binding Texture");

  glActiveTexture(GL_TEXTURE0 + wgl->font->index);
  checkForGLError("Activating Texture");
  glBindTexture(wgl->font->type, wgl->font->id);
  checkForGLError("Binding Texture");

  glBindVertexArray(wgl->vertexArrayObject);
  checkForGLError("Binding vertex array object (In render)");
  glDrawArrays(GL_TRIANGLES, 0, 6);
  checkForGLError("Drawing arrays");
}