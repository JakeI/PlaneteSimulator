#include "globals.h"

void initGlobals() {
	UniformColoredModle::init();
	LinearBars::init();
	txt::init();
	TexturedModle::init();
	TexturedQuad::init();
	Arrow::init();
	Sphere::init();
	LitColoredModle::init();
	CordSystem::init();
}

void setCourentView(RotatableCamera* c) {
	setCourentView(c->getProjection(), c->getView(), c->getPosition());
}

void setCourentView(glm::mat4 projection, glm::mat4 view, glm::vec3 eyePos) {
	glm::mat4 projectionView = projection*view;
	UniformColoredModle::pojectionView = projectionView;
	ColoredModle::projectionView = projectionView;

	lcm::GlobalParam gp;
	gp.set(eyePos, projection, view);
	LitColoredModle::setViewParam(&gp);
}

void setLighttSources(int number, LightSource* lightsources) {
	if (number >= 1) {
		lcm::LightParam lp;
		lp.set(lightsources[0].pos);
		LitColoredModle::setLightParam(&lp);
	}
}

void deleteGlobals() {
	UniformColoredModle::close();
	LinearBars::close();
	txt::close();
	TexturedModle::close();
	TexturedQuad::close();
	Arrow::close();
	Sphere::close();
	CordSystem::close();
}

int globalDebugParam = 0;