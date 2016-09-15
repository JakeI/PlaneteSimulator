#pragma once

#include "StdAfx.h"
#include "shaders\LoadShaderProg.h"
#include <sstream>
#include <map>
#include "Loader.h"

class TxtShaderProg : public ShaderProg {
private:
	void LoadUniformID();

	void start(GLuint vao, GLuint vboXY, GLuint vboUV, float fbColor[8]);
	void stop();

	GLuint fbColorID;

public:
	TxtShaderProg() : 
		ShaderProg("shaders\\Txt.vertex", "shaders\\Txt.fragment") {
		LoadUniformID();
	}
	~TxtShaderProg(){}
	void draw(GLuint vao, GLuint vboXY, GLuint vboUV, 
		int length, GLenum mode, float fbColor[8]);
	void drawPrestarted(GLuint vao, GLuint vboXY, GLuint vboUV, 
		int length, GLenum mode, float fbColor[8]);
	void useTexture(GLuint tbo);
};

class txt {
private:
	static TxtShaderProg* shader;
	static unsigned int nextFreeKey;
public:
	typedef std::map<unsigned int, txt*> indexedMap;
private:
	static indexedMap areas;
	static bool wasInitialised;
	static GLuint tbo;

	unsigned int key;
	std::stringstream sText;
	GLuint vao, vboXY, vboUV;
	int length;
	bool wasWritten;
	float fourBackGroundColor[8];
public:
	static void init();
	static void close();
	static void draw();

	txt();
	~txt();

	static enum StreamElements { endl, dec, hex, oct };
	txt& operator<<(const StreamElements &s);
	template <typename T>
	txt& operator<<(const T &x) {
		sText << x;
		return *this;
	}

	void write(float X, float Y, float fFontSize = 0.05f);
	void clearText();

	void drawThisText();

	void setColors(glm::vec4 fourground, glm::vec4 background);
};