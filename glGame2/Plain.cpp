#include "Plain.h"

StaticPlainShaderProg Plain::shader;
bool Plain::shaderWasLoaded = false;
int Plain::countInstances = 0;

Plain::Plain() {
	vaoID = 0;
	vboVertecies = 0;
	vboColors = 0;

	countInstances++;

	if (!shaderWasLoaded) {
		shader.genShaderProg();
		shaderWasLoaded = true;
	}
}

Plain::~Plain()
{
	if (glIsVertexArray(vaoID))
		glDeleteVertexArrays(1, &vaoID);
	deleteBufferIfExisting(&vboVertecies);
	deleteBufferIfExisting(&vboColors);
	deleteBufferIfExisting(&eboID);

	if (countInstances == 1) {
		shader.delShaderProg();
		shaderWasLoaded = false;
	}
	countInstances--;
}

void Plain::prepare(bool startOrEnd)
{
	shader.use(startOrEnd);
	if (startOrEnd) {
		glBindVertexArray(vaoID);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboVertecies);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboColors);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}
	else {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void Plain::perpareAndDraw(float timeMs, glm::mat4 matrix)
{
	prepare(true);
	uploadMatrix(matrix);
	shader.uploadTime(timeMs);
	draw();
	prepare(false);
}

void Plain::draw()
{
	//glDrawArrays(GL_LINES, 0, 9);
	glDrawElements(GL_LINES, countIndecies, GL_UNSIGNED_INT, (void*)0);
}

void Plain::genData(int CountVertex) {
	genArrayBufferIfnExisting();
	deleteBufferIfExisting(&vboVertecies);

	int countVertecies = (CountVertex+1)*(CountVertex+1);
	float* pos = new float[countVertecies*2];

	float step = 2.0f / (float)CountVertex;
	int index = 0;
	for (float r = -1.0f; r <= 1.0f; r += step) for (float s = -1.0f; s <= 1.0f; s += step) { // r in z and s in x
		pos[index++] = s;
		pos[index++] = r;
	}
	vboVertecies = loadToVBO(countVertecies * 2, pos);

	delete[] pos;

	deleteBufferIfExisting(&vboColors);

	float* cols = new float[countVertecies * 3];

	for (int i = 0; i < countVertecies * 3; i++)
		cols[i] = 0.0f;

	vboColors = loadToVBO(countVertecies * 3 - 6, cols);
	
	delete[] cols;

	countIndecies = 2 * ((CountVertex + 1)*((2 * (CountVertex + 1)) - 2));
	GLuint* indecies = new GLuint[countIndecies];
	index = 0;
	for (int j = 0; j < CountVertex+1; j++) {
		for (int i = 0; i < CountVertex; i++) {
			indecies[index] = (j*(CountVertex+1) + i);
			indecies[index + 1] = (j*(CountVertex+1) + (i + 1));
			index += 2;
		}
	}
	
	for (int j = 0; j < CountVertex; j++) {
		for (int i = 0; i < CountVertex+1; i++) {
			indecies[index] = (j)*(CountVertex+1) + i;
			indecies[index + 1] = (j + 1)*(CountVertex + 1) + i;
			index += 2;
		}
	}

	eboID = loadToEBO(countIndecies, indecies);

	delete[] indecies;
}

void Plain::genArrayBufferIfnExisting() {
	if (!glIsVertexArray(vaoID))
		glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

void Plain::deleteBufferIfExisting(GLuint* id) {
	if (glIsBuffer(*id))
		glDeleteBuffers(1, id);
}

void Plain::uploadMatrix(glm::mat4 mvpMatrix) {
	shader.uploadMatrix(mvpMatrix);
}
