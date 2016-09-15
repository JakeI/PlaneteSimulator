#pragma once

#include "StdAfx.h"
#include "Arrow.h"

struct StdCustomData {
	float graviConst;
};

typedef glm::vec3(*ForceFunc)(glm::vec3, glm::vec3*, float*, int, void*);

class ForceField {
private:
	struct FieldPoint {
		glm::vec3 p, v;
		FieldPoint();
	};
	void initArr(FieldPoint* arr, int len);
	float findMaxLength(FieldPoint* arr, int len);
	void drawArr(FieldPoint* arr, int len, float vFactor = 1.0f);
public:
	static StdCustomData stdCustomData;
	static glm::vec3 defaultForceFunc(glm::vec3 x, glm::vec3* p, float* m, int l, void* costom);

	int length;
	glm::vec3* positions;
	float* masses;
	void* costomData;
	ForceFunc f;

	float xMin, xMax, yMin, yMax, zMin, zMax;
	int xStep, yStep, zStep;
	glm::vec4 color;

	float maxArrowLength;
	
	ForceField() {
		length = 0;
		positions = nullptr;
		costomData = &ForceField::stdCustomData;
		f = ForceField::defaultForceFunc;
		xMin = -30.0f, xMax = 30.0f, yMin = -30.0f, yMax = 30.0f, zMin = -30.0f, zMax = 30.0f;
		xStep = 9, yStep = 9, zStep = 9;
		color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
		maxArrowLength = 1.0f;
	}
	~ForceField(){}

	void draw();
	void setData(glm::vec3* pos, float* masses, int length) {
		positions = pos;
		this->length = length;
		this->masses = masses;
	};
	void setCostomData(void* c) { costomData = c; };
	void setFunction(ForceFunc F) { f = F; }
};