#pragma once
#include "Modle.h"
#include "Loader.h"
#include "shaders\LoadShaderProg.h"

class StaticColoredModleShaderProg :
	public ShaderProg {
private:
	GLuint matrixID;
	void LoadMatirxUniformID() {
		matrixID = glGetUniformLocation(ProgrammID, "MVP");
	}
public:
	StaticColoredModleShaderProg() {
		matrixID = 0;
		usingExternalDestructor = true;
	}
	void genShaderProg() {
		Load("shaders\\ColoredModle.vertex", "shaders\\ColoredModle.fragment");
		LoadMatirxUniformID();
		usingExternalDestructor = true;
	}
	void delShaderProg() {
		delShaders();
	}
	void uploadMatrix(glm::mat4 matrix) {
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);
	}
};

class ColoredModle :
	public Modle
{
private:
	
	static StaticColoredModleShaderProg shader;
	static bool shaderWasLoaded;
	static int countInstances;


	GLuint vaoID;
	GLuint vboVertecies, vboColors;
	int countVertecies;
	GLenum renderingMode;

	void genArrayBufferIfnExisting();
	void deleteBufferIfExisting(GLuint* id);
public:
	static glm::mat4 projectionView;

	ColoredModle();
	~ColoredModle();
	void prepare(bool startOrEnd);
	void perpareAndDraw(glm::mat4 matrix);
	void draw();
	void setData(int numFloat, float* positions, float* colors);
	void uploadMatrix(glm::mat4 mvpMatrix);

	void setRenderingMode(GLenum mode) { renderingMode = mode; }
};

