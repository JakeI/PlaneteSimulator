#include "TexturedModle.h"

void TexturedShaderProg::LoadUniformID() {
	matrixID = glGetUniformLocation(ProgrammID, "MVP");
}

void TexturedShaderProg::start(GLuint vao, GLuint vboXYZ, GLuint vboUV, GLuint tbo) {
	
	glBindTexture(GL_TEXTURE_2D, tbo);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboXYZ);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}
void TexturedShaderProg::stop() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void TexturedShaderProg::draw(GLuint vao, GLuint vboXYZ, GLuint vboUV, GLuint tbo,
	int length, GLenum mode, glm::mat4 mvp) {
	use(true);
	drawPrestarted(vao, vboXYZ, vboUV, tbo, length, mode, mvp);
	use(false);
}

void TexturedShaderProg::uploadMatrix(glm::mat4 matrix) {
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
}

void TexturedShaderProg::drawPrestarted(GLuint vao, GLuint vboXYZ, GLuint vboUV,
	GLuint tbo, int length, GLenum mode, glm::mat4 mvp) {
	start(vao, vboXYZ, vboUV, tbo);
	uploadMatrix(mvp);
	glDrawArrays(mode, 0, length);
	stop();
}

TexturedShaderProg* TexturedModle::shader;
bool TexturedModle::wasInitialised = false;

void TexturedModle::init() {
	if (wasInitialised)
		return;

	shader = new TexturedShaderProg();
	wasInitialised = true;
}
void TexturedModle::close() {
	if (!wasInitialised)
		return;

	delete shader;
	wasInitialised = false;
}

TexturedModle::TexturedModle() {
	vao = 0;
	vboXYZ = 0;
	vboUV = 0;
	tbo = 0;
	length = 0;
}

TexturedModle::~TexturedModle() {}

void TexturedModle::draw(glm::mat4 mvp) {
	shader->draw(vao, vboXYZ, vboUV, tbo, length, GL_QUADS, mvp);
}