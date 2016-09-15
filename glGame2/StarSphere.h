#pragma once

#include "Sphere.h"

class StarSphere {
private:
	Sphere** spheres;
	glm::vec3* xes;
	int countSpheres;

	void initSpheres(float rMin, float rMax);
	Sphere* createSphere();
	glm::vec3 createPos(float rMin, float rMax, float xAngleMin,
		float xAngleMax, float zAngleMin, float zAngleMax);
	lcm::DrawParam creatDrawParam();

public:
	StarSphere(float rMin, int count);
	~StarSphere();

	void draw();
};
