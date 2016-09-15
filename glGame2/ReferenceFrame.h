#pragma once

#include "StdAfx.h"
#include "Arrow.h"

class ReferenceFrame {
public:
	
	glm::vec3 vilocity;
	glm::vec3 position;
	float scale;

	ReferenceFrame();
	ReferenceFrame(glm::vec3 vilocity, glm::vec3 position);
	~ReferenceFrame();

	glm::vec3 transform(glm::vec3 v, glm::vec3 atR);
};