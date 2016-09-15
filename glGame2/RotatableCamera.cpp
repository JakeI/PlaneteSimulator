#include "RotatableCamera.h"

RotatableCamera::Dimention::Dimention() {
	width = 1;
	hight = 1;
}
void RotatableCamera::Dimention::set(ViewDimention* viewDimention) {
	width = viewDimention->width;
	hight = viewDimention->hight;
}

RotatableCamera::MouseCoordinates::MouseCoordinates(int X, int Y) {
	x = X;
	y = Y;
}

void RotatableCamera::MouseCoordinates::set(int X, int Y) {
	x = X;
	y = Y;
}

RotatableCamera::Angles::Angles() {
	horizontal = 0.0f;
	vertical = 0.0f;
	viewDirectionChanged = false;
}

RotatableCamera::RotatableCamera() : Camera() {
	FoV = glm::radians(45.0f);
	mProjection = glm::mat4(1.0f);
	DeltaCamarePos = glm::vec3(0.0f, 0.0f, 0.0f);
	updateProjection();
	absVelocity = -0.005;
}

void RotatableCamera::updateProjection() {
	if (dimention.width != 0 && dimention.hight != 0) {
		mProjection = glm::perspective(FoV,
			(float)dimention.width / (float)dimention.hight,
			0.1f, 400.0f);
	}
}

void RotatableCamera::updateScreenSize(ViewDimention* viewDimentions) {
	dimention.set(viewDimentions);
	updateProjection();
}

void RotatableCamera::updateDeltaPos(POSITION_DELTA_TYPE p) {
	
	DeltaCamarePos = glm::vec3(0.0f, 0.0f, 0.0f);

	if (p & POSITION_DELTA_A) { 
		DeltaCamarePos += glm::vec3(0.0f, 0.0f, 1.0f); 
	}

	if (p & POSITION_DELTA_D) {
		DeltaCamarePos += glm::vec3(0.0f, 0.0f, -1.0f);
	}

	if (p & POSITION_DELTA_S) {
		DeltaCamarePos += glm::vec3(1.0f, 0.0f, 0.0f);
	}

	if (p & POSITION_DELTA_W) {
		DeltaCamarePos += glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	if (p & POSITION_DELTA_DOWN) {
		DeltaCamarePos += glm::vec3(0.0f, 1.0f, 0.0f);
	}

	if (p & POSITION_DELTA_UP) {
		DeltaCamarePos += glm::vec3(0.0f, -1.0f, 0.0f);
	}

	absVelocity = p & POSITION_DELTA_SMALL ? -0.0002f : -0.005f;
}

void RotatableCamera::dragRoatate(MouseCoordinates* oldMouse,
	MouseCoordinates* newMouse)
{
	angles.horizontal += (float)(oldMouse->x - newMouse->x);
	angles.vertical += (float)(oldMouse->y - newMouse->y);
	angles.viewDirectionChanged = true;
}

void RotatableCamera::updateFoV(float deltaMouseWheel) {
	FoV += deltaMouseWheel * radPerWheelLine;
	updateProjection();
}

void RotatableCamera::updateMatrix() {
	
	if (angles.viewDirectionChanged) {
		setDirection(mouseSpeed*angles.horizontal,
			mouseSpeed*angles.vertical);
		setRightDircection(mouseSpeed*angles.horizontal);
		angles.viewDirectionChanged = false;
	}
	
	Camera::updateMatrix();
}

glm::mat4 RotatableCamera::getMatrix() {
	return mProjection * Camera::getMatrix();
}

void RotatableCamera::movePlayer(float virtualDeltaTms) {
	Camera::movePlayer(absVelocity*DeltaCamarePos*virtualDeltaTms);
}