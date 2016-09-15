#pragma once

#include "StdAfx.h"

class PlanetePositionCalculator {
private:

public:
	PlanetePositionCalculator();
	~PlanetePositionCalculator();

	struct Vec3Pair {
		glm::vec3 x, dxdt;
	};
	virtual void getPositionVilocity(Vec3Pair* p, float time){}
};