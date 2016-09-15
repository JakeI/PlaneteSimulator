#pragma once

#include "UniformColoredModle.h"
#include "Loader.h"

class PlaneteTrace : private UniformColoredModle {
private:

	const static int traceLength = 1000;
	int usedLength;
	int courserPos;
	struct Pos {
		float x, y, z;
		void set(glm::vec3 v);
		void set(float f);
		Pos();
	} trace[traceLength];
	bool traceFull;

public:
	PlaneteTrace();
	~PlaneteTrace();

	void setColor(glm::vec4 col);

	void draw();
	void add(glm::vec3 pos);
};