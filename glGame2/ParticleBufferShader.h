#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"

class ParticleBufferShader :
	public ShaderProg
{
private:
	GLuint mvpID, mID, vID, pID, eyeID;
	GLuint sID;

	void start(GLuint vao, GLuint vboPartPos, GLuint vboCenterPos, GLuint vboNorm, GLuint vboScale, GLuint vboN);
	void stop();

	void LoadUniformIDs();

public:
	ParticleBufferShader();
	~ParticleBufferShader();

	struct BufferParam {
		//vaos
		GLuint vao;
		//vbos
		GLuint vboPartPos;
		//instance vbos
		GLuint vboCenterPos;
		GLuint vboNorm;
		GLuint vboScale;
		GLuint vboN;
		//rendering param
		int length;
		//Functions
		BufferParam();
		void set(GLuint vboCenterPos, GLuint vboNorm, GLuint vboScale, GLuint vboN, int length);
	};

	struct DrawParam {
		GLuint tbo;
		int S;
		DrawParam();
		void set(GLuint tbo, int S);
	};

	void draw(BufferParam* bufferParam, DrawParam* drawParam, glm::mat4 modle = glm::mat4(1.0f));

	GlobalParam globalParam;

private:
	void upload(DrawParam* param);
	void upload(GlobalParam* param, glm::mat4 modle = glm::mat4(1.0f));
};