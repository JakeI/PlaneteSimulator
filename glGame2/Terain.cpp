#include "Terain.h"



Terain::Terain()
{
	float sources[] = {
		//  x		y		T
			4.0f,	0.0f,	10.0f,
			-4.0f, 0.0f,	10.0f,
	};
	int sourceEntries = sizeof(sources) / sizeof(float) / 3;

	const int sizeX = 256, sizeY = 256;
	int length = sizeX*sizeY;
	GLfloat* data = new GLfloat[length*3];
	float startX = -30.0f, endX = 30.0f, startY = -30.0f, endY = 30.0f;
	
	float xStep = (endX - startX) / sizeX, yStep = (endY - startY) / sizeY;
	for (int i = 0; i < sizeY; i++) for (int j = 0; j < sizeX; j++) {
			int index = (i*sizeY + j) * 3;
			float X = xStep*i + startX;
			float Z = yStep*j + startY;
			data[index] = X;
			data[index + 2] = Z;
			float Y = -3.0f;
			for (int i = 0; i < sourceEntries; i++) {
				int index = 3 * i;
				float rx = X - sources[index], ry = Z + sources[index + 1];
				Y += 0.5f*sin(sqrt(rx*rx + ry*ry)*2.0f*M_PI / sources[index + 2]);
			}
			data[index + 1] = Y;
	}
	
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vboVertex = loadToVBO(3 * length, data);

	delete[] data;

	GLfloat* colorData = new GLfloat[length * 3];

	for (int i = 0; i < length * 3; i++) {
		colorData[i] = 1.0f;
	}

	vboColor = loadToVBO(3 * length, colorData);

	delete[] colorData;

	GLfloat* colorDataB = new GLfloat[length * 3];

	for (int i = 0; i < length * 3; i++) {
		colorDataB[i] = 0.0f;
	}

	vboColorBlack = loadToVBO(3 * length, colorDataB);

	delete[] colorDataB;

	indexLenght = 6 * (sizeY - 1)*(sizeX - 1);
	GLuint* indecies = new GLuint[indexLenght];
	GLuint quad[6] = { 0, 1, sizeX,		sizeX, 1, sizeX+1, };
	for (int j = 0; j < (sizeY - 1); j++) for (int i = 0; i < (sizeX - 1); i++) {
		int k = (j*(sizeY-1) + i) * 6;
		int qIndex = j*sizeY + i;
		indecies[k] = quad[0] + qIndex;
		indecies[k + 1] = quad[1] + qIndex;
		indecies[k + 2] = quad[2] + qIndex;

		indecies[k + 3] = quad[3] + qIndex;
		indecies[k + 4] = quad[4] + qIndex;
		indecies[k + 5] = quad[5] + qIndex;
	}

	eboElemnets = loadToEBO(indexLenght, indecies);

	delete[] indecies;

	indexLineLength = 2*(sizeY*(sizeX-1))+2*(sizeX*(sizeY-1));
	GLuint* indeciesLine = new GLuint[indexLineLength];
	int index = 0;
	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX - 1; i++) {
			indeciesLine[index] = j*sizeY + i;
			indeciesLine[index + 1] = j*sizeY + (i + 1);
			index += 2;
		}
	}
	for (int j = 0; j < sizeY - 1; j++) {
		for (int i = 0; i < sizeX; i++) {
			indeciesLine[index] = (j)*sizeY + i;
			indeciesLine[index + 1] = (j+1)*sizeY + i;
			index += 2;
		}
	}

	eboLines = loadToEBO(indexLineLength, indeciesLine);

	delete[] indeciesLine;
}


Terain::~Terain()
{
	glDeleteBuffers(1, &vboVertex);
	glDeleteBuffers(1, &vboColor);
	glDeleteBuffers(1, &eboElemnets);
	glDeleteBuffers(1, &vboColorBlack);
	glDeleteBuffers(1, &eboLines);
	glDeleteVertexArrays(1, &vao);
}

void Terain::draw()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	//glDrawArrays(GL_LINES, 0, 64);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboElemnets);
	glDrawElements(GL_TRIANGLES, indexLenght, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vboColorBlack);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboLines);
	glDrawElements(GL_LINES, indexLineLength, GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
