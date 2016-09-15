#pragma once

#include "Camera.h"

typedef int POSITION_DELTA_TYPE;
#define POSITION_DELTA_NONE		(POSITION_DELTA_TYPE) 0
#define POSITION_DELTA_A		(POSITION_DELTA_TYPE) 2
#define POSITION_DELTA_D		(POSITION_DELTA_TYPE) 4
#define POSITION_DELTA_S		(POSITION_DELTA_TYPE) 8
#define POSITION_DELTA_W		(POSITION_DELTA_TYPE) 16
#define POSITION_DELTA_UP		(POSITION_DELTA_TYPE) 32
#define POSITION_DELTA_DOWN		(POSITION_DELTA_TYPE) 64
#define POSITION_DELTA_SMALL	(POSITION_DELTA_TYPE) 128


class RotatableCamera : public Camera {
public:

	static struct ViewDimention {
		int x, y, width, hight;
	};

private:

	float FoV;
	const float radPerWheelLine = M_PI / 180.0;
	glm::mat4 mProjection;
	glm::vec3 DeltaCamarePos;
	float absVelocity;// in world_space_units/ms
	const float mouseSpeed = 0.004f;

	struct Angles {
		float horizontal, vertical;
		bool viewDirectionChanged;
		Angles();
	} angles;

	static struct Dimention {
		int width, hight;
		Dimention();
		void set(ViewDimention* viewDimention);
	};
	Dimention dimention;

	void updateProjection();

public:

	static struct MouseCoordinates {
		int x;
		int y;
		MouseCoordinates(int X, int Y);
		void set(int X, int Y);
	};

	RotatableCamera();

	~RotatableCamera(){}

	void updateScreenSize(ViewDimention* viewDimentions);
	void updateDeltaPos(POSITION_DELTA_TYPE p);
	void dragRoatate(MouseCoordinates* oldMouse, MouseCoordinates* newMouse);
	void updateFoV(float deltaMouseWheel);

	void updateMatrix() override;
	glm::mat4 getMatrix() override;

	void movePlayer(float virtualDeltaTms);

	glm::vec3 getPos() { return position; }
	glm::vec3 getDirection() { return direction; }
	glm::vec3 getUp() { return up; }

	glm::mat4 getProjection() { return mProjection; }
	glm::mat4 getView() { return matrix; }
};