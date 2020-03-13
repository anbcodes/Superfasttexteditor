
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#if defined(PRODUCTION)
#define checkForGLError(message)
#define logCompileErrors(shader)
#else
#define checkForGLError(message) optionalCheckForGLErrors(message)
#define logCompileErrors(shader) optionalLogCompileErrors(shader)
#endif

typedef struct WrappedGL {
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;
  // GLuint vertexBufferObject;
  GLuint vertexArrayObject;
} WrappedGL;

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
  wgl->vertexShader = glCreateShader(GL_VERTEX_SHADER);
  checkForGLError("Creating vertex shader");
  glShaderSource(wgl->vertexShader, 1, &vertexShaderSource, 0);
  checkForGLError("Adding source to vertex shader");
  glCompileShader(wgl->vertexShader);
  checkForGLError("Compiling vertex shader");
  logCompileErrors(wgl->vertexShader);
  glAttachShader(wgl->program, wgl->vertexShader);
  checkForGLError("Attaching vertex shader");
  glDeleteShader(wgl->vertexShader);
  checkForGLError("Deleting vertex shader");  
}

void wglLoadFragmentShaderFromFile(WrappedGL *wgl, const char *path) {
  const char *fragmentShaderSource = readFile(path);
  printf("fragment Shader:\n%s\n", fragmentShaderSource);
  wgl->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  checkForGLError("Creating fragment shader");
  glShaderSource(wgl->fragmentShader, 1, &fragmentShaderSource, 0);
  checkForGLError("Adding source to fragment shader");
  glCompileShader(wgl->fragmentShader);
  checkForGLError("Compiling fragment shader");
  logCompileErrors(wgl->fragmentShader);
  glAttachShader(wgl->program, wgl->fragmentShader);
  checkForGLError("Attaching fragment shader");
  glDeleteShader(wgl->fragmentShader);
  checkForGLError("Deleting fragment shader");
}

void wglInitVertexArray(WrappedGL *wgl) {
  glGenVertexArrays(1, &wgl->vertexArrayObject);
  checkForGLError("Initializing vertex array");  
}

void wglRender(WrappedGL *wgl) {
  glClear(GL_COLOR_BUFFER_BIT);
  checkForGLError("Clearing");

  glBindVertexArray(wgl->vertexArrayObject);
  checkForGLError("Binding vertex array object (In render)");
  glDrawArrays(GL_TRIANGLES, 0, 6);
  checkForGLError("Drawing arrays");
}