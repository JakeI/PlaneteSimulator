#include "UniformColoredModle.h"

void UniformColoredModleShader::start(GLuint vao, GLuint vbo) {
	use(true);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void UniformColoredModleShader::stop() {
	glDisableVertexAttribArray(0);

	use(false);
}

UniformColoredModleShader::UniformColoredModleShader()
	: ShaderProg("shaders\\UniformColoredModleShader.vertex", 
		"shaders\\UniformColoredModleShader.fragment") {
	LoadMatirxUniformID();
}

UniformColoredModleShader::~UniformColoredModleShader() {}

void UniformColoredModleShader::uploadMatrix(glm::mat4 matrix) {
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
}

void UniformColoredModleShader::uploadColor(glm::vec4 col) {
	glUniform4fv(colorID, 1, &col[0]);
}

void UniformColoredModleShader::LoadMatirxUniformID() {
	matrixID = glGetUniformLocation(ProgrammID, "MVP");
	colorID = glGetUniformLocation(ProgrammID, "color");
}

void UniformColoredModleShader::draw(GLuint vao, GLuint vbo, 
	glm::vec4 col, glm::mat4 mvp, int length, GLenum mode) 
{
	start(vao, vbo);

	uploadColor(col);
	uploadMatrix(mvp);

	glDrawArrays(mode, 0, length);

	stop();
}

void UniformColoredModleShader::draw(GLuint vao, GLuint vbo, GLuint ebo, 
	glm::vec4 col, glm::mat4 mvp, int length, GLenum mode) {

	start(vao, vbo);

	uploadColor(col);
	uploadMatrix(mvp);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(mode, length, GL_UNSIGNED_INT, (void*)0);

	stop();
}



UniformColoredModleShader* UniformColoredModle::shader;
bool UniformColoredModle::wasInitialised = false;
glm::mat4 UniformColoredModle::pojectionView = glm::mat4(1.0f);


void UniformColoredModle::init() {
	if (wasInitialised)
		return;
	shader = new UniformColoredModleShader();
	wasInitialised = true;
}

void UniformColoredModle::close() {
	if (!wasInitialised)
		return;
	delete shader;
	wasInitialised = false;
}


UniformColoredModle::UniformColoredModle() {
	vao = 0;
	vbo = 0;
	length = 0;
	col = glm::vec4(0.5f);
}

UniformColoredModle::~UniformColoredModle() {

}

void UniformColoredModle::draw(GLenum mode, glm::mat4 modle) {
	shader->draw(vao, vbo, col, pojectionView*modle, length, mode);
}

void UniformColoredModle::draw(GLenum mode, GLuint ebo, glm::mat4 modle) {
	shader->draw(vao, vbo, ebo, col, pojectionView*modle, length, mode);
}