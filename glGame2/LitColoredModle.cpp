#include "LitColoredModle.h"

LitColoredModleShader::BufferParam::BufferParam() {
	set(0, 0, 0, 0, 0);
}

void LitColoredModleShader::BufferParam::set(GLuint vao, GLuint vboPos, GLuint vboNorm,
	int length, GLenum mode)
{
	this->vao = vao;
	this->vboPos = vboPos;
	this->vboNorm = vboNorm;
	this->length = length;
	this->mode = mode;
	
	this->ebo = 0;
	this->useEBO = false;
}

void LitColoredModleShader::BufferParam::set(GLuint vao, GLuint vboPos, GLuint vboNorm,
	GLuint ebo, int length, GLenum mode)
{
	set(vao, vboPos, vboNorm, length, mode);
	this->ebo = ebo;
	this->useEBO = true;
}


LitColoredModleShader::DrawParam::DrawParam() {
	set(glm::vec4(1.0f), 0.0f, 0.0f, 0.0f);
}

void LitColoredModleShader::DrawParam::set(glm::vec4 col, 
	float specular, float defuse, float ambient) 
{
	this->col = col;
	this->specular = specular;
	this->defuse = defuse;
	this->ambinet = ambient;
}

void LitColoredModleShader::BufferParam::set(BufferParam* bp) {
	vao = bp->vao;
	vboPos = bp->vboPos;
	vboNorm = bp->vboNorm;
	ebo = bp->ebo;
	useEBO = bp->useEBO;
	length = bp->length;
	mode = bp->mode;
}



void LitColoredModleShader::start(GLuint vao, GLuint vboPos, GLuint vboNorm) {
	use(true);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboNorm);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void LitColoredModleShader::stop() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	use(false);
}

void LitColoredModleShader::LoadUniformIDs() {
	mvpID = glGetUniformLocation(ProgrammID, "MVP");
	mID = glGetUniformLocation(ProgrammID, "M");
	pID = glGetUniformLocation(ProgrammID, "P");
	vID = glGetUniformLocation(ProgrammID, "V");

	colorID = glGetUniformLocation(ProgrammID, "color");

	specularID = glGetUniformLocation(ProgrammID, "specular");
	defuseID = glGetUniformLocation(ProgrammID, "defuse");
	ambientID = glGetUniformLocation(ProgrammID, "ambient");

	lightPosID = glGetUniformLocation(ProgrammID, "lightPos");
	eyePosID = glGetUniformLocation(ProgrammID, "eyePos");
}

LitColoredModleShader::LitColoredModleShader() 
	: ShaderProg("shaders\\LitColoredModle.vertex",
		"shaders\\LitColoredModle.fragment") 
{
	LoadUniformIDs();
}

LitColoredModleShader::~LitColoredModleShader() {
	
}

void LitColoredModleShader::draw(BufferParam* bufferParam, DrawParam* drawParam, glm::mat4 modle) {
	start(bufferParam->vao, bufferParam->vboPos, bufferParam->vboNorm);

	upload(drawParam);
	upload(&globalParam, modle);
	upload(&lightParam);

	if (bufferParam->useEBO && bufferParam->ebo != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferParam->ebo);
		glDrawElements(bufferParam->mode, bufferParam->length, GL_UNSIGNED_INT, (void*)0);
	} else {
		glDrawArrays(bufferParam->mode, 0, bufferParam->length);
	}

	stop();
}

void LitColoredModleShader::upload(DrawParam* param) {
	glUniform4fv(colorID, 1, &param->col[0]);
	glUniform1f(defuseID, param->defuse);
	glUniform1f(specularID, param->specular);
	glUniform1f(ambientID, param->ambinet);
}

void LitColoredModleShader::upload(GlobalParam* param, glm::mat4 modle) {
	glUniform3fv(eyePosID, 1, &param->eyePos[0]);
	glm::mat4 mvp = param->projection*param->view*modle;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(mID, 1, GL_FALSE, &modle[0][0]);
	glUniformMatrix4fv(vID, 1, GL_FALSE, &param->view[0][0]);
	glUniformMatrix4fv(pID, 1, GL_FALSE, &param->projection[0][0]);
}

void LitColoredModleShader::upload(LightParam* param) {
	glUniform3fv(lightPosID, 1, &param->lightPos[0]);
}



LitColoredModleShader* LitColoredModle::shader;
bool LitColoredModle::wasInitialised = false;


void LitColoredModle::init() {
	if (wasInitialised)
		return;
	wasInitialised = true;

	shader = new LitColoredModleShader();
}

void LitColoredModle::close() {
	if (!wasInitialised)
		return;
	wasInitialised = false;

	delete shader;
}


LitColoredModle::LitColoredModle() {
}

LitColoredModle::~LitColoredModle() {
}

void LitColoredModle::draw(lcm::DrawParam* drawParam, glm::mat4 modle) {
	shader->draw(&bufferParam, drawParam, modle);
}

void LitColoredModle::setViewParam(lcm::GlobalParam* param) {
	shader->globalParam.set(param);
}

void LitColoredModle::setLightParam(lcm::LightParam* param) {
	shader->lightParam.set(param);
}