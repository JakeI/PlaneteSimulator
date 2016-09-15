#pragma once

#include "TexturedModle.h"
#include "Loader.h"

class TexturedQuad
{
private:
	static TexturedModle* texturedModle;
	static bool wasInitialised;

	GLuint tbo;
public:

	static void init();
	static void close();

	TexturedQuad();
	~TexturedQuad();

	void draw(glm::mat4 mvp);

	void setTbo(GLuint Tbo) { tbo = Tbo; }
};