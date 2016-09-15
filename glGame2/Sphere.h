#pragma once

#include "LitColoredModle.h"
#include "Loader.h"
#include "globals.h"

class Sphere {
private:
	static const int plainRotSteps = 16;
	static const int countVertPerCircle = 32;
	static const int countVertecies = plainRotSteps*countVertPerCircle;

	static LitColoredModle* modle;
	static bool wasInitialised;

	static float cosTriangle(float p) {
		p = p - (2 * (int)(p / 2));
		return (2 * abs(1 - abs(p))) - 1;
	}
	static float sinTriangle(float p) {
		return cosTriangle(p - 0.5f);
	}

	static void genArrayBufferIfnExisting();
	static void deleteBufferIfExisting(GLuint* id);

	static void initPositionData();
	static void initIndecies();
	static void initIndecies(lcm::BufferParam* bp, int skip);

	static lcm::BufferParam bpBig, bpSmall;

public:
	static void init();
	static void close();

	float radius;

	lcm::DrawParam drawParam;

	Sphere();
	~Sphere();
	void prepare(bool startOrEnd);

	enum SpereSize { BIG, SMALL};
	void draw(glm::vec3 pos, SpereSize ss = BIG);
};