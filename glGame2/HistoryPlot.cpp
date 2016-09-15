#include "HistoryPlot.h"

HistoryPlot::HistoryPlot() : UniformColoredModle() {
	length = 0;
	usedLength = 0;
	minX = -1.0f;
	maxX = 1.0f;
	minY = -1.0f;
	maxY = 1.0f;
}

HistoryPlot::HistoryPlot(int length, glm::vec4 color, 
	int minX, int maxX, int minY, int maxY) 
	: UniformColoredModle() {
	this->length = length;
	data = new float[length];
	memset(data, 0.0f, length);

	this->col = color;
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}

HistoryPlot::~HistoryPlot() {
	delete[] data;
}

void HistoryPlot::add(float v) {
	for (int i = length-1; i > 0; i--) {
		data[i] = data[i - 1];
	}
	data[0] = v;
	usedLength = usedLength >= length ? length : usedLength + 1;
}

void HistoryPlot::render() {
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	
	int lenV = usedLength * 3;
	float* v = new float[lenV];
	float step = usedLength / (maxX - minX - BarWidth);
	for (int i = 0; i < usedLength; i++) {
		v[i] = step*i + minX;
		v[i + 1] = data[i];
		v[i + 2] = 0.0f;
	}

	this->vbo = loadToVBO(lenV, v);
	
	this->draw(GL_LINE_STRIP);
	LinearBars::output(data[0], minX, minX+BarWidth, &glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)[0], -1.0f);

	delete v;
	glDeleteBuffers(1, &this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}