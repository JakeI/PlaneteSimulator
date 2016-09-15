#pragma once

#include "PlanetePositionCalculator.h"
#include <sstream>

class KeplerCalculator : public PlanetePositionCalculator {
public:
	struct Pair {
		float value;
		float ddt;
		Pair();
		void set(float Value, float Derivitiv);
		float aprox(float timePassed);
		void toRad();
	};
	Pair _a, _e, _I, _L, _wbar, _Omega;
	bool convertedToRad;
private:
	struct State {
		float a;
		float e;
		float I;
		float L;
		float omega;
		float Omega;
		void set(KeplerCalculator* kc, float time);
		float M;
		float E;
		void solvekeplersEq();
		float getTrueAnomaly();
		float getRadius();
		glm::mat3 getZXZ();
	};

	glm::vec3 getPos(float time);
	void getPsoitionVilocityRSchwartz(PlanetePositionCalculator::Vec3Pair* p, float time);
	void getPositionVilocityEMStandish(PlanetePositionCalculator::Vec3Pair* p, float time);

public:
	KeplerCalculator();
	~KeplerCalculator();

	void getPositionVilocity(PlanetePositionCalculator::Vec3Pair* p, float time) override;
	void readln(std::stringstream* ss);
};