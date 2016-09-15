#pragma once

#include "UniformColorShaderProg.h"
#include "Loader.h"

#ifndef LINE_BARS
#define LINE_BARS
class LinearBars {
private:
	static GLuint vao, vbo;
	static int vboLength;
	static bool wasInitialised;
	static UniformColorShaderProg* shader;
public:
	static void init() {
		if (wasInitialised)
			return;
		wasInitialised = true;
		//make Sqare for LinearOutputBars
		const static float posSquare[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		vboLength = (sizeof(posSquare) / sizeof(float))/3;
		vbo = loadToVBO(vboLength*3, (float*)posSquare);
		shader = new UniformColorShaderProg();
	}
	static void close() {
		if (!wasInitialised)
			return;
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		delete shader;
	}
	static void output(float value, float from, float to, float* col, float startAt = 0.0f) {
		glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(from, startAt, 0.0f));
		m = glm::scale(m, glm::vec3(to - from, value, 0.0f));
		shader->draw(vao, vbo, col, m, vboLength, GL_TRIANGLES);
	}
};

#endif