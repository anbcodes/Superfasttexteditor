#ifndef WRAPED_GL_H
#define WRAPED_GL_H
#endif

#ifdef WRAPED_GL_H

typedef struct WrappedGLTexture {
  GLuint id;
  GLuint location;
  const GLchar * name;
  GLuint type;
  int index;
  GLsizei width;
  GLsizei height;
  GLfloat * data;
} WrappedGLTexture;

typedef struct WGLUniformVec2 {
  const GLchar * name;
  GLuint location;
  GLfloat value[2];
} WGLUniformVec2;

typedef struct WGLUniformInt {
  const GLchar * name;
  GLuint location;
  GLint value;
} WGLUniformInt;

typedef struct WrappedGL {
  GLuint program;
  GLuint vertexArrayObject;
  WrappedGLTexture* data;
  WrappedGLTexture* font;
  WGLUniformVec2* mouse;
  WGLUniformInt* mouseButton;
  WGLUniformInt* mouseDown;
} WrappedGL;

void wglInit();

WrappedGL * wglCreateContext();

void wglCreateProgram(WrappedGL *wgl);

void wglLinkProgram(WrappedGL *wgl);

void wglUseProgram(WrappedGL* wgl);

void wglLoadVertexShaderFromFile(WrappedGL *wgl, const char *path);

void wglLoadFragmentShaderFromFile(WrappedGL *wgl, const char *path);

void wglInitVertexArray(WrappedGL *wgl);

void wglInitTexture(WrappedGL *wgl, WrappedGLTexture* texture, const GLchar * name);

void wglGenTexture2D(WrappedGLTexture* texture, GLsizei width, GLsizei height, GLubyte * data);

void wglGenTexture1D(WrappedGLTexture *texture, GLsizei width, GLubyte *data);

void wglInitUniformVec2(WrappedGL *wgl, WGLUniformVec2* uniform);

void wglInitUniformInt(WrappedGL *wgl, WGLUniformInt* uniform);

void wglUpdateUniforms(WrappedGL *wgl);

void wglRender(WrappedGL *wgl);

#endif