#include "OnPlaneteCamera.h"

OnPlaneteCamera::OnPlaneteCamera() : RotatableCamera() {
	planeteNo = 0;
}

OnPlaneteCamera::~OnPlaneteCamera() {

}

void OnPlaneteCamera::update(PlaneteBuffer* pb) {
	if (planeteNo >= (int)(pb->planets.size()-1)) {
		planeteNo = 0;
	} else if (planeteNo < 0) {
		planeteNo = pb->planets.size() - 2;
	}

	Planet::PositionNormalPair pn;
	pb->planets.at(planeteNo)->getPosAndNormalAt(angles.horizontal, angles.vertical, &pn);
	placeAt(pn.p);
	setUpDirection(pn.n);
}

void OnPlaneteCamera::updateDeletaPlanetePos(PLNETE_POSITION_DELTA_TYPE ppdt) {
	if (ppdt & PLANETE_POSITION_DELTA_UP) {
		angles.horizontal += M_PI / 60.0f;
	}
	if (ppdt & PLANETE_POSITION_DELTA_DOWN) {
		angles.horizontal -= M_PI / 60.0f;
	}
	if (ppdt & PLANETE_POSITION_DELTA_RIGHT) {
		angles.vertical += M_PI / 60.0f;
	}
	if (ppdt & PLANETE_POSITION_DELTA_LEFT) {
		angles.vertical -= M_PI / 60.0f;
	}
	if (ppdt & PLANETE_POSITION_DELTA_NEXT) {
		planeteNo -= 1;
	}
	if (ppdt & PLANETE_POSITION_DELTA_PREV) {
		planeteNo += 1;
	}
}