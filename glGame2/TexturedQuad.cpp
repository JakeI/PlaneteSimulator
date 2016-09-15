#include "TexturedQuad.h"

TexturedModle* TexturedQuad::texturedModle;
bool TexturedQuad::wasInitialised = false;

void TexturedQuad::init() {
	
	if (wasInitialised)
		return;

	texturedModle = new TexturedModle();

	static const float xyz[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	
	glGenVertexArrays(1, &texturedModle->vao);
	texturedModle->length = (sizeof(xyz) / sizeof(float)) / 3;
	texturedModle->vboXYZ = loadToVBO(sizeof(xyz) / sizeof(float), (float*)xyz);
	
	static const float uv[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	texturedModle->vboUV = loadToVBO(sizeof(uv) / sizeof(float), (float*)uv);

	texturedModle->tbo = 0;

	wasInitialised = true;
}

void TexturedQuad::close() {
	if (!wasInitialised)
		return;

	glDeleteVertexArrays(1, &texturedModle->vao);
	glDeleteBuffers(1, &texturedModle->vboXYZ);
	glDeleteBuffers(1, &texturedModle->vboUV);

	delete texturedModle;

	wasInitialised = false;
}

TexturedQuad::TexturedQuad() {
	tbo = 0;
}

TexturedQuad::~TexturedQuad() {

}

void TexturedQuad::draw(glm::mat4 mvp) {
	if (wasInitialised) {
		texturedModle->tbo = tbo;
		texturedModle->draw(mvp);
	}
}