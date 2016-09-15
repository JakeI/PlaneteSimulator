#include "LoadShaderProg.h"

GLuint ShaderProg::LoadShader(char* path, int type) {
	std::fstream in(path, std::ios::in);
	if (in.fail()) {
		MessageBox(NULL, path, "fatal error shader Path reading", MB_OK);
		return 0;
	}
	in.seekg(0, std::ios::end);
	int len = ((int)(in.tellg())) + 1;
	in.seekg(0, std::ios::beg);
	char* source = new char[len];
	for (int i = 0; i < len; i++)
		source[i] = '\0';
	in.read(source, len);
	in.close();
	
	/*int error = glutInit();
	if(errer == )*/

	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, NULL);
	delete[] source;
	glCompileShader(shaderID);
	GLint compiled;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		int logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* errorMessage = new char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, errorMessage);
		MessageBox(NULL, errorMessage, "Compile Error", MB_OK);
		return 0;
	}
	return shaderID;
}

ShaderProg::ShaderProg(char* VertexShaderFilePath,
	char* FragmentShaderFilePath) {

	Load(VertexShaderFilePath, FragmentShaderFilePath);
}

void ShaderProg::Load(char* VertexShaderFilePath,
	char* FragmentShaderFilePath) {
	VertexShaderID = 0;
	FragmentShaderID = 0;
	ProgrammID = 0;

	VertexShaderID = LoadShader(VertexShaderFilePath, GL_VERTEX_SHADER);
	FragmentShaderID = LoadShader(FragmentShaderFilePath, GL_FRAGMENT_SHADER);

	ProgrammID = glCreateProgram();
	glAttachShader(ProgrammID, VertexShaderID);
	glAttachShader(ProgrammID, FragmentShaderID);

	glLinkProgram(ProgrammID);

	GLint linked;
	glGetProgramiv(ProgrammID, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		int logLength;
		glGetProgramiv(ProgrammID, GL_INFO_LOG_LENGTH, &logLength);
		char* errorMessage = new char[logLength];
		glGetProgramInfoLog(ProgrammID, logLength, NULL, errorMessage);
		MessageBox(NULL, errorMessage, "Compile Error", MB_OK);
	}

	glValidateProgram(ProgrammID);
}

ShaderProg::~ShaderProg() {
	if(!usingExternalDestructor)
		delShaders();
}

void ShaderProg::delShaders() {
	use(false);
	if (ProgrammID != 0 && VertexShaderID != 0)
		glDetachShader(ProgrammID, VertexShaderID);
	if (ProgrammID != 0 && FragmentShaderID != 0)
		glDetachShader(ProgrammID, FragmentShaderID);
	if (VertexShaderID != 0)
		glDeleteShader(VertexShaderID);
	if (FragmentShaderID != 0)
		glDeleteShader(FragmentShaderID);
	if (ProgrammID != 0)
		glDeleteProgram(ProgrammID);
}

void ShaderProg::use(bool u) {
	glUseProgram(u ? ProgrammID : 0);
}

GlobalParam::GlobalParam() {
	set(glm::vec3(0.0f), glm::mat4(1.0f), glm::mat4(1.0f));
}

void GlobalParam::set(
	glm::vec3 eyePos, glm::mat4 projection, glm::mat4 view)
{
	this->eyePos = eyePos;
	this->view = view;
	this->projection = projection;
}

void GlobalParam::set(GlobalParam* p) {
	set(p->eyePos, p->projection, p->view);
}


LightParam::LightParam() {
	set(glm::vec3(0.0f));
}

void LightParam::set(glm::vec3 lightPos) {
	this->lightPos = lightPos;
}

void LightParam::set(LightParam* p) {
	set(p->lightPos);
}