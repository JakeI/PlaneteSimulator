#pragma once

#include "RotatableCamera.h"
#include "PlaneteBuffer.h"

typedef int PLNETE_POSITION_DELTA_TYPE;
#define PLANETE_POSITION_DELTA_NONE		(PLNETE_POSITION_DELTA_TYPE) 0
#define PLANETE_POSITION_DELTA_UP		(PLNETE_POSITION_DELTA_TYPE) 2
#define PLANETE_POSITION_DELTA_DOWN		(PLNETE_POSITION_DELTA_TYPE) 4
#define PLANETE_POSITION_DELTA_LEFT		(PLNETE_POSITION_DELTA_TYPE) 8
#define PLANETE_POSITION_DELTA_RIGHT	(PLNETE_POSITION_DELTA_TYPE) 16
#define PLANETE_POSITION_DELTA_NEXT		(PLNETE_POSITION_DELTA_TYPE) 32
#define PLANETE_POSITION_DELTA_PREV		(PLNETE_POSITION_DELTA_TYPE) 64

class OnPlaneteCamera : public RotatableCamera {
public://For debugging

	struct Angles {
		float horizontal, vertical;
		Angles() { horizontal = M_PI_2; vertical = 3*M_PI_2; }
	} angles;
	int planeteNo;

public:

	OnPlaneteCamera();
	~OnPlaneteCamera();

	void update(PlaneteBuffer* pb);
	void updateDeletaPlanetePos(PLNETE_POSITION_DELTA_TYPE ppdt);
};