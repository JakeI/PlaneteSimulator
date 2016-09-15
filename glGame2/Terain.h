#pragma once

#include "StdAfx.h"
#include "Loader.h"

struct terainVertex {
	float x, y, z;
};

class Terain
{
private:
	GLuint vao;
	GLuint vboVertex;
	GLuint vboColor;
	GLuint eboElemnets;
	GLuint vboColorBlack;
	GLuint eboLines;

	int indexLenght;
	int indexLineLength;
public:
	Terain();
	~Terain();
	void draw();
};

