#pragma once

//
//	TODO: Finish class
//

#include "StdAfx.h"
#include "UniformColoredModle.h"

class HistoryPlot : private UniformColoredModle {
private:
	int length;
	int usedLength;
	float* data;
	float minX, maxX, minY, maxY;

	const float BarWidth = 0.05f;
public:

	HistoryPlot();
	HistoryPlot(int length, glm::vec4 color, int minX = -1.0f, 
		int maxX = 1.0f, int minY = -1.0f, int maxY = 1.0f);
	~HistoryPlot();

	void add(float v);
	void render();
};