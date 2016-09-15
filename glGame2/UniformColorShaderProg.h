#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"
class UniformColorShaderProg :
	public ShaderProg
{
private:
	GLuint matrixID, colorID;

	void start(GLuint vao, GLuint vbo) {
		use(true);

		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	void stop(){
		glDisableVertexAttribArray(0);

		use(false);
	}

public:
	UniformColorShaderProg()
		: ShaderProg("shaders\\Sphere.vertex", "shaders\\Sphere.fragment") { //TODO: Write Spesific shaders
		LoadMatirxUniformID();
	}
	~UniformColorShaderProg() {}
	void uploadMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
	}
	void uploadColor(float* col) {
		glUniform3fv(colorID, 1, col);
	}
	void LoadMatirxUniformID() {
		matrixID = glGetUniformLocation(ProgrammID, "MVP");
		colorID = glGetUniformLocation(ProgrammID, "color");
		//MessageBox(NULL, std::to_string(matrixID).c_str(), "MatrixID", MB_OK);
	}
	void draw(GLuint vao, GLuint vbo, float* col, glm::mat4 mvp, int length, GLenum mode) {
		start(vao, vbo);

		uploadColor(col);
		uploadMatrix(mvp);

		glDrawArrays(mode, 0, length);

		stop();
	}
	void drawInterval(GLuint vao, GLuint vbo, float* col, glm::mat4 mvp,
		GLenum mode, int* intervals, int contIntervals) {
		start(vao, vbo);

		uploadColor(col);
		uploadMatrix(mvp);

		for (int i = 0; i < contIntervals; i++) {
			int index = 2 * i;
			glDrawArrays(mode, intervals[index], intervals[index + 1]);
		}

		stop();
	}
};

