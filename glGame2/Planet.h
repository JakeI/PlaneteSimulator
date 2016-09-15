#pragma once

#include "Sphere.h"
#include "UniformColorShaderProg.h"
#include "txt.h"
#include "PlanetePositionCalculator.h"
#include "PlaneteTrace.h"
#include "Arrow.h"
#include "ReferenceFrame.h"

class Planet{
private:
	glm::vec3 pos, vilocity;
	float mass;

	Sphere sphere;
	PlaneteTrace trace;

	struct PosCalculater {
		PlanetePositionCalculator* calc;
		bool use;
		PosCalculater() { use = false; calc = nullptr; }
		~PosCalculater() { delete calc; }
	} posCalculater;

	void setColor(glm::vec3 col);
	void setColor(glm::vec4 col);

	bool isLight;

public:

	glm::vec4 getColor(bool complement = false);

	std::string name;

	Planet();
	void setAll(float Mass, float Radius, glm::vec3 col, glm::vec3 initialPos, glm::vec3 initailVilocity);
	void setMRC(float Mass, float Radius, glm::vec3 col);
	void setInitial(glm::vec3 initialPos, glm::vec3 initialVilocity);
	~Planet();
	void draw();
	void drawVilosityVector(ReferenceFrame* rf);
	glm::vec3 getVilosityVector(ReferenceFrame* rf);
	void setToLightSource(bool value) { isLight = value; }

	void drawTrace();
	void addToTrace();

	void respondeToFroce(glm::vec3 F, float timeMS);
	void colideWith(Planet* p);
	void reflectAt(glm::vec3 normalHat, float energyLoss);

	void add(PlanetePositionCalculator* pc);
	void obtainInitials(float time);

	glm::vec3 getPos() { return pos; }
	glm::vec3 getVilocity() { return vilocity; }
	float getMass() { return mass; }
	float getRadius() { return sphere.radius; }
	float getKineticEnergy();
	glm::vec3 getMomentum() { return mass*vilocity; }
	bool isLightSource() { return isLight; }

	struct PositionNormalPair {
		glm::vec3 p, n;
		PositionNormalPair() { p = glm::vec3(0.0f); n = glm::vec3(0.0f); }
	};
	void getPosAndNormalAt(float horizontal, float vertical, PositionNormalPair* pn);

	void getInfo(std::string* str);
};
