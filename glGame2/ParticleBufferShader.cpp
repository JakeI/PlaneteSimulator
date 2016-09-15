#include "ParticleBufferShader.h"

void ParticleBufferShader::BufferParam::set(GLuint vboCenterPos, GLuint vboNorm, GLuint vboScale, GLuint vboN, int length) {
	// TODO: Dont forget getting an extended Unit squere into the buffer
}

void ParticleBufferShader::DrawParam::set(GLuint tbo, int S) {
	this->tbo = tbo;
	this->S = S;
}


void ParticleBufferShader::start(GLuint vao, GLuint vboPartPos, GLuint vboCenterPos, GLuint vboNorm, GLuint vboScale, GLuint vboN) {
	use(true);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboPartPos);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboCenterPos);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vboNorm);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vboScale);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, vboN);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(4, 1);

}

void ParticleBufferShader::stop() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	use(false);
}

void ParticleBufferShader::LoadUniformIDs() {
	mvpID = glGetUniformLocation(ProgrammID, "MVP");
	mID = glGetUniformLocation(ProgrammID, "M");
	pID = glGetUniformLocation(ProgrammID, "P");
	vID = glGetUniformLocation(ProgrammID, "V");
	eyeID = glGetUniformLocation(ProgrammID, "eye");

	sID = glGetUniformLocation(ProgrammID, "S");
}

ParticleBufferShader::ParticleBufferShader()
	: ShaderProg("shaders\\LitColoredModle.vertex",
		"shaders\\LitColoredModle.fragment")
{
	LoadUniformIDs();
}

ParticleBufferShader::~ParticleBufferShader() {

}

void ParticleBufferShader::draw(BufferParam* bufferParam, DrawParam* drawParam, glm::mat4 modle) {

	start(bufferParam->vao, bufferParam->vboPartPos, bufferParam->vboCenterPos, 
		bufferParam->vboNorm, bufferParam->vboScale, bufferParam->vboN);

	upload(drawParam);
	upload(&globalParam, modle);
	//TODO: Bind Texture

	glDrawArrays(GL_QUADS, 0, bufferParam->length);

	stop();
}

void ParticleBufferShader::upload(DrawParam* param) {
	//TODO: write this
}

void ParticleBufferShader::upload(GlobalParam* param, glm::mat4 modle) {
	glUniform3fv(eyeID, 1, &param->eyePos[0]);
	glm::mat4 mvp = param->projection*param->view*modle;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(mID, 1, GL_FALSE, &modle[0][0]);
	glUniformMatrix4fv(vID, 1, GL_FALSE, &param->view[0][0]);
	glUniformMatrix4fv(pID, 1, GL_FALSE, &param->projection[0][0]);
}