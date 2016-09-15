#include "LinearBars.h"

bool LinearBars::wasInitialised = false;
GLuint LinearBars::vao = 0;
GLuint LinearBars::vbo = 0;
int LinearBars::vboLength = 0;
UniformColorShaderProg* LinearBars::shader;