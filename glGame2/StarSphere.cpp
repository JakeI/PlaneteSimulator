#include "StarSphere.h"

void StarSphere::initSpheres(float rMin, float rMax) {
	this->spheres = new Sphere*[this->countSpheres];
	this->xes = new glm::vec3[this->countSpheres];
	for (int i = 0; i < this->countSpheres; i++) {
		this->spheres[i] = createSphere();
		this->xes[i] = createPos(rMin, rMax, 0.0f, 2.0f*(float)M_PI, 0.0f, (float)M_PI);
		this->spheres[i]->drawParam = creatDrawParam();
	}
}


inline float getRand(float max, float min) {
	return (((float)rand() / (float)RAND_MAX)*(max - min)) + min;
}

inline float getRandSin(float max, float min) {
	return ((1-sin(((float)rand() / (float)RAND_MAX)*M_PI))*(max - min)) + min;
}
inline float getRandCos(float max, float min) {
	return (1.0f-pow(cos((((float)rand() / (float)RAND_MAX)-0.5)*(M_PI)),2)*(max - min)) + min;
}
inline float getRandPlusMinus() {
	return ((float)rand() / (float)RAND_MAX) > 0.5f ? 1.0f : -1.0f;
}

Sphere* StarSphere::createSphere(){
	Sphere* s = new Sphere();
	s->radius = getRand(0.1f, 0.3f);
	return s;
}

glm::vec3 StarSphere::createPos(float rMin, float rMax, float xAngleMin,
	float xAngleMax, float zAngleMin, float zAngleMax) {
	float r = getRand(rMax, rMin);
	float xAngle = getRand(xAngleMax, xAngleMin);
	float zAngle = getRand(zAngleMax, zAngleMin);
	return glm::vec3(r*sin(zAngle)*cos(xAngle), r*sin(zAngle)*sin(xAngle), r*cos(zAngle));
}

lcm::DrawParam StarSphere::creatDrawParam() {
	lcm::DrawParam dp;
	dp.set(glm::vec4(10.0f, 10.0f, 10.0f, 10.0f), 0.6, 0.6, 10.0);
	return dp;
}

StarSphere::StarSphere(float rMin, int count) {
	srand(5064676876);
	this->countSpheres = count;
	initSpheres(rMin, 1.5f*rMin);
}

StarSphere::~StarSphere() {
	for (int i = 0; i < this->countSpheres; i++)
		delete this->spheres[i];
	delete[] this->spheres;
	delete[] this->xes;
}

void StarSphere::draw() {
	for (int i = 0; i < this->countSpheres; i++) {
		this->spheres[i]->draw(this->xes[i], Sphere::SMALL);
	}
}