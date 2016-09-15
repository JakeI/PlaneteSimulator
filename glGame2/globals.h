#pragma once

#include "StdAfx.h"
#include "LinearBars.h"
#include "txt.h"
#include "TexturedModle.h"
#include "TexturedQuad.h"
#include "UniformColoredModle.h"
#include "Arrow.h"
#include "Sphere.h"
#include "Camera.h"
#include "RotatableCamera.h"
#include "LitColoredModle.h"
#include "ColoredModle.h"
#include "CordSystem.h"

void initGlobals();
void setCourentView(RotatableCamera* c);
void setCourentView(glm::mat4 projection, glm::mat4 view, glm::vec3 eyePos);

struct LightSource {
	glm::vec3 pos;
};
void setLighttSources(int number, LightSource* lightsources);

void deleteGlobals();


extern int globalDebugParam;