#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"
#include "LinearBars.h"

class UniformColoredModleShader :
	public ShaderProg
{
private:
	GLuint matrixID, colorID;

	void start(GLuint vao, GLuint vbo);
	void stop();

public:
	UniformColoredModleShader();
	~UniformColoredModleShader();
	void uploadMatrix(glm::mat4 matrix);
	void uploadColor(glm::vec4 col);
	void LoadMatirxUniformID();
	void draw(GLuint vao, GLuint vbo, glm::vec4 col, glm::mat4 mvp, int length, GLenum mode);
	void draw(GLuint vao, GLuint vbo, GLuint ebo, glm::vec4 col, glm::mat4 mvp, int length, GLenum mode);
};

class UniformColoredModle {
private:
	
	static UniformColoredModleShader* shader;
	static bool wasInitialised;

public:

	static void init();
	static void close();

	GLuint vao, vbo;
	int length;
	glm::vec4 col;

	UniformColoredModle();
	~UniformColoredModle();

	void draw(GLenum mode, glm::mat4 modle = glm::mat4(1.0f));
	void draw(GLenum mode, GLuint ebo, glm::mat4 modle = glm::mat4(1.0f));

	static glm::mat4 pojectionView;
};
