#pragma once
#include "Modle.h"

#include "Loader.h"
#include "shaders\LoadShaderProg.h"

class StaticPlainShaderProg :
	public ShaderProg {
private:
	GLuint matrixID;
	GLuint timeMsID;
	void LoadUniformID() {
		matrixID = glGetUniformLocation(ProgrammID, "MVP");
		timeMsID = glGetUniformLocation(ProgrammID, "timeMS");
	}
public:
	StaticPlainShaderProg() {
		matrixID = 0;
		usingExternalDestructor = true;
	}
	void genShaderProg() {
		Load("shaders\\Plain.vertex", "shaders\\Plain.fragment");
		LoadUniformID();
		usingExternalDestructor = true;
	}
	void delShaderProg() {
		delShaders();
	}
	void uploadMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
	}
	void uploadTime(float timeMS) {
		glUniform1f(timeMsID, timeMS);
	}
};


class Plain :
	public Modle
{
private:
	static StaticPlainShaderProg shader;
	static bool shaderWasLoaded;
	static int countInstances;

	GLuint vaoID;
	GLuint vboVertecies, vboColors;
	GLuint eboID;
	int countIndecies;

	void genArrayBufferIfnExisting();
	void deleteBufferIfExisting(GLuint* id);

public:
	Plain();
	~Plain();
	void prepare(bool startOrEnd);
	void perpareAndDraw(float timeMs, glm::mat4 matrix);
	void draw();
	void genData(int CountVertex);
	void uploadMatrix(glm::mat4 mvpMatrix);
};

