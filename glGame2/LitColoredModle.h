#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"

class LitColoredModleShader :
	public ShaderProg
{
private:
	GLuint mvpID, mID, vID, pID;
	GLuint colorID;
	GLuint specularID, defuseID, ambientID;
	GLuint lightPosID, eyePosID;

	void start(GLuint vao, GLuint vboPos, GLuint vboNorm);
	void stop();

	void LoadUniformIDs();

public:
	LitColoredModleShader();
	~LitColoredModleShader();

	struct BufferParam {
		GLuint vao;
		GLuint vboPos;
		GLuint vboNorm;
		GLuint ebo;
		bool useEBO;
		int length;
		GLenum mode;
		BufferParam();
		void set(GLuint vao, GLuint vboPos, GLuint vboNorm,
			int length, GLenum mode);
		void set(GLuint vao, GLuint vboPos, GLuint vboNorm,
			GLuint ebo, int length, GLenum mode);
		void set(BufferParam* bp);
	};

	struct DrawParam {
		glm::vec4 col;
		float specular;
		float defuse;
		float ambinet;
		DrawParam();
		void set(glm::vec4 col, float specular, float defuse, float ambient);
	};

	void draw(BufferParam* bufferParam, DrawParam* drawParam, glm::mat4 modle = glm::mat4(1.0f));
	
	GlobalParam globalParam;

	LightParam lightParam;

private:
	void upload(DrawParam* param);
	void upload(GlobalParam* param, glm::mat4 modle = glm::mat4(1.0f));
	void upload(LightParam* param);
};

namespace lcm {
	typedef LitColoredModleShader::BufferParam BufferParam;
	typedef LitColoredModleShader::DrawParam DrawParam;
	typedef GlobalParam GlobalParam;
	typedef LightParam LightParam;
}

class LitColoredModle {
private:

	static LitColoredModleShader* shader;
	static bool wasInitialised;

public:

	static void init();
	static void close();

	lcm::BufferParam bufferParam;

	LitColoredModle();
	~LitColoredModle();

	void draw(lcm::DrawParam* drawParam, glm::mat4 modle = glm::mat4(1.0f));

	static void setViewParam(lcm::GlobalParam* param);
	static void setLightParam(lcm::LightParam* param);
};
