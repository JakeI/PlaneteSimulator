#pragma once

#include "StdAfx.h"
#include "UniformColoredModle.h"
#include "Loader.h"

class Arrow {
private:
	static UniformColoredModle* modle;
	static bool wasInitialised;

	static GLuint ebo;

public:
	Arrow() {
	}
	~Arrow() {
	}

	static void init();
	static void close();

	static void draw(glm::vec3 pos, glm::vec3 vector, glm::vec4 col);
};