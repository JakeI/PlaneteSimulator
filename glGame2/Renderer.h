#pragma once

#include "StdAfx.h"
#include "Loader.h"
#include "GameShaderProg.h"
#include "RotatableCamera.h"
#include "Terain.h"
#include "ColoredModle.h"
#include "Plain.h"
#include "PlaneteBuffer.h"
#include "FrameBufferQuad.h"
#include "OnPlaneteCamera.h"
#include "UserInput.h"
#include "ReferenceFrame.h"
#include "CordSystem.h"
#include "StarSphere.h"
#include "globals.h"

class Renderer
{
private:
	char INPUT_PLANETE_PATH[256];

	GLuint vaoID, vaoTid;
	GLuint vboID, vboColorID, vboTid;
	
	RotatableCamera* flyingCamera;
	OnPlaneteCamera* planeteCamera;

	GameShaderProg* shader;

	Terain* terain;
	ColoredModle* cube;
	Plain* plain;

	const float spin = M_PI/1000;//in rad/ms
	float rotation;

	static struct ViewDimention {
		int x, y, width, hight;
		void set(UserInput::WindowSize* windowSize);
	};
	ViewDimention viewDimention;

	HWND hWnd;

	float timeMs;

	PlaneteBuffer* planeteBuffer;

	bool incrementPlanets;
	bool toggleIncrementPlanets;

	CordSystem* cordSystem;

	int planetSpeed; // move to PlaneteBuffer

	bool cameraIsOnPanet;

	txt* text;

	FrameBufferQuad* frameQuad;

	bool ForceFieldOn;
	ForceField* forceField;

	void renderSliders();
	void renderPlanets(int exeptFor = -1);
	void renderViewVector(RotatableCamera* carmera);
	txt* currentSelectionText;
	void renderSelctionText();
	void renderCameraPointer();

	void incrementAllPlanets(float virtualDeltaTms);
	void updateOnPlaneteCamera();

	bool drawVilocityArrows;
	bool drawJupiterVilocityArrow;

	void renderVilocityArrows(int planetNo = -1);

	int lookedAtPlanetId;

	bool drawStarSphere;
	StarSphere* starSphere;

	bool drawTraces;

public:

	Renderer(char* path);
	~Renderer();

	void draw();
	void increment(float virtualTimems, float virtualDeltaTms);
	void updateUserInput(UserInput* userInput, INPUT_CHANGE_BUFFER changeID);
};
