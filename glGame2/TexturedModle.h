#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"

class TexturedShaderProg : public ShaderProg {
private:
	void LoadUniformID();

	void start(GLuint vao, GLuint vboXYZ, GLuint vboUV, GLuint tbo);
	void stop();

	GLint matrixID;

public:
	TexturedShaderProg() :
		ShaderProg("shaders\\Textur.vertex", "shaders\\Textur.fragment") {
		LoadUniformID();
	}
	~TexturedShaderProg() {}
	void uploadMatrix(glm::mat4 matrix);
	void draw(GLuint vao, GLuint vboXYZ, GLuint vboUV, GLuint tbo,
		int length, GLenum mode, glm::mat4 mvp);
	void drawPrestarted(GLuint vao, GLuint vboXYZ, GLuint vboUV,
		GLuint tbo, int length, GLenum mode, glm::mat4 mvp);
};

class TexturedModle {
private:
	static TexturedShaderProg* shader;
	static bool wasInitialised;
public:
	static void init();
	static void close();

	GLuint vao, vboXYZ, vboUV, tbo;
	int length;

	TexturedModle();
	~TexturedModle();
	void draw(glm::mat4 mvp);
};
