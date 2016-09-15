#pragma once

#include "StdAfx.h"

GLuint loadToVBO(int countFloats, float* data);
GLuint loadToEBO(int countInts, unsigned int* indecies);

void triangleStripToLines(int countUIntTriangleStrip, 
	unsigned int* triangleStripIndex, unsigned int* prealocketedLinesBuffer);
// allocate memory for prealockatedLinesBuffer as 
// new unsigned int[(countUIntTriangleStrip-3)*6];

void trianglesToLines(int countUIntTriangles, unsigned int* trianglesIndex,
	unsigned int* prealockatedLinesBuffer); // untested
// allocate memory for prealockatedLinesBuffer as 
// new unsigned int[countUIntTriangleStrip*2];

bool eq(float a, float b, float epsilon = 0.01f);
bool eq(glm::vec3 a, glm::vec3 b, float epsilon = 0.01f);

struct textureDim {
	int width;
	int hight;
};
struct textureFlags {
	GLint internalFormat;
	GLenum format;
	GLenum type;
};
GLuint loadTexture(textureDim* dim, textureFlags* flags, char* data);
int loadBMP(char* file, char** image, textureDim* dim);
