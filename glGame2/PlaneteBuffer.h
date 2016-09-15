#pragma once

#include "Sphere.h"
#include "Planet.h"
#include <vector>
#include <fstream>
#include <string>
#include "UniformColorShaderProg.h"
#include <sstream>
#include "GlobalConditions.h"
#include "KeplerCalculator.h"
#include "globals.h"
#include "ReferenceFrame.h"
#include "ForceField.h"

#define MIN_ABS_DIST 0.005f

class PlaneteBuffer {
private:

	void incrementAllOnece(float timeMS);
	void PlaneteBuffer::checkForColisions();

	UniformColorShaderProg* traceShader;

	float getPotEnergy(int index);

	GlobalConditions* globalConditions;

	void readFile(char* csvFilePath);
	void readKartesianData(std::ifstream* file);
	void readKeplerData(std::ifstream* file);

	void setAllPlanetesToTime(float timeInTunits);
	void incrementPlanetsDiffEq(float timeMS);

	int getCurrentLightSourceIndex();

public:
	std::vector<Planet*> planets;

	PlaneteBuffer(char* csvInputFilePath);
	~PlaneteBuffer();

	void incrementPlanets(float timeMS);
	void drawPlanets(int exeptFor = -1);
	void drawPlanetTraces();

	float getTotalEnergy();
	float getTotalKinetikEnergy();
	float getTotalPotentialEnergy();
	glm::vec3 getTotalMomentum();

	void intRefFrame(ReferenceFrame* rf, int planet = -1);
	void drawAllVilosityArrows(ReferenceFrame* rf, int exeptFor = -1);

	Planet* getPlaneteByName(char* szName);

	struct AverageForce {
		glm::vec3 force;
		int count;
		AverageForce() { zero(); }
		void zero() { force = glm::vec3(0.0f); count = 0; }
		void takeAverage() { force /= (float)count; }
		void add(glm::vec3 F) { force += F; count++; }
		void addAll(glm::vec3* F, int length) {
			for (int i = 0; i < length; i++)
				add(F[i]);
		}
	}averageForce;

	void getLightSource(int* number, LightSource** lightsource);

	StdCustomData forceFieldData;
	void initForceField(ForceField* FF);
	void updateForceField(ForceField* FF);
};