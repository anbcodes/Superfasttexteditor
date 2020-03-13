typedef struct WrappedGL {
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;
  GLuint vertexBufferObject;
  GLuint vertexArrayObject;
} WrappedGL;

WrappedGL * wglCreateContext();

void wglInit();

void wglCreateProgram(WrappedGL *wgl);

void wglLinkProgram(WrappedGL *wgl);

void wglUseProgram(WrappedGL *wgl);

void wglLoadVertexShaderFromFile(WrappedGL *wgl, const char *path);

void wglLoadFragmentShaderFromFile(WrappedGL *wgl, const char *path);

void wglInitVertexBuffer(WrappedGL *wgl);
void wglInitVertexArray(WrappedGL *wgl);

void wglAddVertices(WrappedGL *wgl, float vertices[]);

void wglRender(WrappedGL *wgl);