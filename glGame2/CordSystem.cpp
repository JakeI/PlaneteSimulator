#include "CordSystem.h"

const float CordSystem::pos[] = {
	-1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f,			0.0f, 0.0f, 1.0f
};

const float CordSystem::col[] = {
	1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
};

bool CordSystem::wasInitialised = false;
ColoredModle* CordSystem::coloredModle = nullptr;

void CordSystem::init() {
	if (wasInitialised)
		return;
	if (sizeof(pos) / sizeof(pos[0]) != sizeof(col) / sizeof(col[0]))
		return;

	wasInitialised = true;
	coloredModle = new ColoredModle();
	coloredModle->setData(sizeof(pos) / sizeof(pos[0]), (float*)pos, (float*)col);
}

void CordSystem::close() {
	if (!wasInitialised)
		return;
	wasInitialised = false;
	delete coloredModle;
}

CordSystem::CordSystem() {
	scale = glm::vec3(1.0f);
	origenPos = glm::vec3(0.0f);
	showCord = true;
	coloredModle->setRenderingMode(GL_LINES);
}

CordSystem::~CordSystem() {

}

void CordSystem::draw() {
	if (showCord) {
		glm::mat4 m = glm::mat4(1.0f);
		m = glm::translate(m, origenPos);
		m = glm::scale(m, scale);
		coloredModle->perpareAndDraw(m);
	}
}