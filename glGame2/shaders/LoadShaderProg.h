#pragma once

#include "../StdAfx.h"

class ShaderProg {
protected:
	bool usingExternalDestructor;

	int VertexShaderID;
	int FragmentShaderID;
	int ProgrammID;

	GLuint LoadShader(char* path, int type);
	void delShaders();
public:
	ShaderProg() {
		VertexShaderID = 0;
		FragmentShaderID = 0;
		ProgrammID = 0;
		usingExternalDestructor = false;
	}
	ShaderProg(char* VertexShaderFilePath,
		char* FragmentShaderFilePath);
	void Load(char* VertexShaderFilePath,
		char* FragmentShaderFilePath);
	void use(bool u);
	~ShaderProg();
};

struct LightParam {
	glm::vec3 lightPos;
	LightParam();
	void set(glm::vec3 lightPos);
	void set(LightParam* p);
};

struct GlobalParam {
	glm::vec3 eyePos;
	glm::mat4 projection;
	glm::mat4 view;
	GlobalParam();
	void set(glm::vec3 eyePos, glm::mat4 projection, glm::mat4 view);
	void set(GlobalParam* p);
};