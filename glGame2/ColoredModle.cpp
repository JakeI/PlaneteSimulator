#include "ColoredModle.h"

StaticColoredModleShaderProg ColoredModle::shader;
bool ColoredModle::shaderWasLoaded = false;
int ColoredModle::countInstances = 0;
glm::mat4 ColoredModle::projectionView = glm::mat4(1.0f);

ColoredModle::ColoredModle() {
	vaoID = 0;
	vboVertecies = 0;
	vboColors = 0;
	renderingMode = GL_TRIANGLES;

	countInstances++;

	if (!shaderWasLoaded) {
		shader.genShaderProg();
		shaderWasLoaded = true;
	}
}

ColoredModle::~ColoredModle()
{
	if (glIsVertexArray(vaoID))
		glDeleteVertexArrays(1, &vaoID);
	deleteBufferIfExisting(&vboVertecies);
	deleteBufferIfExisting(&vboColors);

	if (countInstances == 1) {
		shader.delShaderProg();
		shaderWasLoaded = false;
	}
	countInstances--;
}

void ColoredModle::prepare(bool startOrEnd)
{
	shader.use(startOrEnd);
	if (startOrEnd) {
		glBindVertexArray(vaoID);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboVertecies);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboColors);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}else {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void ColoredModle::perpareAndDraw(glm::mat4 matrix)
{
	prepare(true);
	uploadMatrix(matrix);
	draw();
	prepare(false);
}

void ColoredModle::draw()
{
	glDrawArrays(renderingMode, 0, countVertecies);
}

void ColoredModle::setData(int numFloat, float* positions, float* colors) {
	genArrayBufferIfnExisting();
	deleteBufferIfExisting(&vboVertecies);	
	vboVertecies = loadToVBO(numFloat, positions);
	deleteBufferIfExisting(&vboColors);
	vboColors = loadToVBO(numFloat, colors);
	countVertecies = numFloat;
}

void ColoredModle::genArrayBufferIfnExisting() {
	if (!glIsVertexArray(vaoID))
		glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

void ColoredModle::deleteBufferIfExisting(GLuint* id) {
	if (glIsBuffer(*id))
		glDeleteBuffers(1, id);
}

void ColoredModle::uploadMatrix(glm::mat4 mvpMatrix) {
	shader.uploadMatrix(projectionView*mvpMatrix);
}
