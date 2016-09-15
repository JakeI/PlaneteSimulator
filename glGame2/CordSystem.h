#pragma once

#include "ColoredModle.h"

class CordSystem {
private:
	const static float pos[];
	const static float col[];
	static bool wasInitialised;
	static ColoredModle* coloredModle;

	glm::vec3 scale;
	glm::vec3 origenPos;
	bool showCord;
public:
	static void init();
	static void close();

	CordSystem();
	~CordSystem();

	void setScale(glm::vec3 Scale) { scale = Scale; }
	void setOriginPos(glm::vec3 OriginPos) { origenPos = OriginPos; }
	void show(bool value) { showCord = value; }
	void toggleShow() { showCord = !showCord; }

	void draw();
};