#include "PlaneteBuffer.h"

PlaneteBuffer::PlaneteBuffer(char* csvInputFilePath) {	
	readFile(csvInputFilePath);
}

PlaneteBuffer::~PlaneteBuffer() {
	for (int i = planets.size()-1; i >= 0; i--) {
		delete planets[i];
	}

	delete globalConditions;
}

void PlaneteBuffer::readFile(char* csvFilePath) {
	
	int errorCode = 0;

	std::ifstream file(csvFilePath);

	globalConditions = new GlobalConditions();

	globalConditions->readTillData(&file);

	switch (globalConditions->headerType) {
	case GlobalConditions::HeaderType::CARTESIAN:
		readKartesianData(&file);
		break;
	case GlobalConditions::HeaderType::KEPLER:
		readKeplerData(&file);
		break;
	case GlobalConditions::HeaderType::UNREADABLE: default:
		// TODO: Add error Message
		break;
	}

	file.close();

	/*if (errorCode != 0) {
	MessageBox(NULL, "Couldn't read the Input File", "BadFile", MB_OK);
	}*/
}

void PlaneteBuffer::readKartesianData(std::ifstream* file) {
	std::string s;
	getline(*file, s); // ignore header, it's for human readability only

	while (!file->eof()) {
		std::string name;
		bool isLight = false;
		char scaleType = 'l';
		float m = 0.0f, R = 0.0f;
		glm::vec3 pos(0.0f, 0.0f, 0.0f), v(0.0f, 0.0f, 0.0f);
		glm::vec3 col(0.0f, 0.0f, 0.0f);

		// TODO: Fix problems with files that don't look as expected
		try {
			*file >> name >> std::boolalpha >> isLight >> scaleType >> m >> R
				>> pos.x >> pos.y >> pos.z >> v.x >> v.y >> v.z
				>> col.r >> col.g >> col.b;
			Planet* p = new Planet();
			p->setAll(m, R*globalConditions->scales.getScale(scaleType), col, pos, v);
			p->setToLightSource(isLight);
			p->name = name;
			planets.push_back(p);
		}catch (std::istream::failure e) {
			MessageBox(NULL, "ERROR: A unexpected error occoured during reading the planete input file. "
				"This my cause serious problmes", "FATAL ERROR", MB_OK);
			break;
		}
	}
}

void PlaneteBuffer::readKeplerData(std::ifstream* file) {
	std::string s;
	getline(*file, s);//Skip commented header
	while (!file->eof()) {
		std::string name;
		float m = 0.0f, R = 0.0f;
		int id = 0;
		char type = '\0';
		glm::vec3 col(0.0f);
		bool isLightSource = false;
		
		getline(*file, s);
		std::stringstream ss(s);
		//TODO: Handle Errors
		ss >> name >> id >> std::boolalpha >> isLightSource >> type 
			>> m >> R >> col.r >> col.g >> col.b;
		Planet* p = new Planet();
		p->setMRC(m, R*globalConditions->scales.getScale(type), col);
		p->setToLightSource(isLightSource);
		KeplerCalculator* pc = new KeplerCalculator();
		pc->readln(&ss);
		p->name = name;
		p->add(pc);
		p->obtainInitials(globalConditions->currentTime);
		planets.push_back(p);
	}
}

void PlaneteBuffer::checkForColisions() {
	for (int i = 0; i < planets.size()-1; i++) {
		for (int j = 0; j < planets.size()-1; j++) {
			if (i == j)
				continue;
			glm::vec3 dist = planets[j]->getPos() - planets[i]->getPos();
			float lenDist = glm::length(dist);
			if ( lenDist < (planets[i]->getRadius()+planets[j]->getRadius())) { // Colision
				switch (globalConditions->colisionModel) {
				case GlobalConditions::ColisionModel::COMBINE:
					planets[i]->colideWith(planets[j]);
					delete planets[j];

					planets.erase(planets.begin() + j);
					break;
				case GlobalConditions::ColisionModel::REFLECT:
				default:
					if (glm::dot(planets[i]->getVilocity(), dist) > 0.0f // FIX PROBLEM WITH VILOCITY == 0
						/*&& glm::dot(planets[i]->getVilocity(), planets[j]->getVilocity()) > 0.0f*/) { //orbits might collide
						dist = glm::normalize(-dist);
						planets[i]->reflectAt(dist, 0.2f);
					}
					break;
				}
			}
		}
	}
}

void PlaneteBuffer::incrementAllOnece(float timeMS) {

	int countPlanet = planets.size()-1;

	glm::vec3* F = new glm::vec3[countPlanet];
	for (int i = 0; i < countPlanet; i++) {
		F[i] = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < countPlanet; i++) {
		for (int j = 0; j < countPlanet; j++) {
			if (i == j)
				continue;
			glm::vec3 dist = planets[j]->getPos() - planets[i]->getPos();// possibly the wrong way arround
			float absDist = glm::length(dist);
			if (absDist < planets[j]->getRadius() + planets[i]->getRadius())//don't let them move closer together
				continue; //TODO count and output the number ot those problems
			F[i] += (planets[j]->getMass()/pow(absDist, 3))*dist;
		}
		F[i] *= globalConditions->gravitationalConstant * planets[i]->getMass();
	}

	for (int i = 0; i < countPlanet; i++) {
		planets[i]->respondeToFroce(F[i], timeMS);
	}

	averageForce.addAll(F, countPlanet);
}

void PlaneteBuffer::incrementPlanets(float timeMS) {
	switch (globalConditions->renderingMethod) {
	case GlobalConditions::RenderingMethod::KEPLERPOS:
		globalConditions->currentTime += globalConditions->ms2tunitFactor * timeMS;
		setAllPlanetesToTime(globalConditions->currentTime);
		break;
	default: case GlobalConditions::RenderingMethod::DIFFEQ:
		incrementPlanetsDiffEq(globalConditions->ms2tunitFactor * timeMS);
		break;
	}
}

void PlaneteBuffer::incrementPlanetsDiffEq(float timeMS) {
	averageForce.zero();

	int increments = ceil(timeMS*globalConditions->incrementsPtunit);// TODO: remove this from of Random Number Generator
	float fracTime = timeMS / increments;
	for (int i = 0; i < increments; i++) {
		checkForColisions(); // TODO: Spend less time checking for colisions
		incrementAllOnece(fracTime);
	}
	for (int i = 0; i < planets.size(); i++) {
		planets[i]->addToTrace();
	}

	averageForce.takeAverage();
}

int PlaneteBuffer::getCurrentLightSourceIndex() {
	int len = planets.size() - 1;
	for (int i = 0; i < len; i++) {
		if (planets[i]->isLightSource()) {
			return i;
		}
	}
	return 0; //First Planete Is Default
}

float PlaneteBuffer::getTotalEnergy() {
	float E = 0.0f;
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		E += 0.5f*getPotEnergy(i) + planets[i]->getKineticEnergy();
	}
	return E;
}

float PlaneteBuffer::getTotalKinetikEnergy() {
	float E = 0.0f;
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		E += planets[i]->getKineticEnergy();
	}
	return E;
}

float PlaneteBuffer::getTotalPotentialEnergy() {
	float E = 0.0f;
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		E += getPotEnergy(i);
	}
	return 0.5f*E;
}

float PlaneteBuffer::getPotEnergy(int index) {
	float MassPerDist = 0.0;
	int countPlanete = planets.size() - 1;
	for (int j = 0; j < countPlanete; j++) {
		if (index == j)
			continue;
		MassPerDist += planets[j]->getMass() / glm::length(planets[j]->getPos()
			- planets.at(index)->getPos());
	}
	return -MassPerDist*globalConditions->gravitationalConstant*planets.at(index)->getMass();
}

glm::vec3 PlaneteBuffer::getTotalMomentum() {
	glm::vec3 I(0.0f, 0.0f, 0.0f);
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		I += planets[i]->getMomentum();
	}
	return I;
}

void PlaneteBuffer::drawPlanets(int exeptFor) {
	int stop = planets.size();
	for (int i = 0; i < stop; i++) {
		if (i == exeptFor)
			continue;
		planets[i]->draw();
	}
}

void PlaneteBuffer::drawPlanetTraces() {
	int stop = planets.size();
	for (int i = 0; i < stop; i++) {
		planets[i]->drawTrace();
	}
}

void PlaneteBuffer::setAllPlanetesToTime(float timeInTunits) {
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		planets[i]->obtainInitials(timeInTunits);
	}
}

void PlaneteBuffer::getLightSource(int* number, LightSource** lightsource) {
	*number = 1;
	(*lightsource)->pos = planets.at(getCurrentLightSourceIndex())->getPos();
}

void PlaneteBuffer::intRefFrame(ReferenceFrame* rf, int planet) {
	int countPlanete = planets.size() - 1;
	if (planet >= 0 && planet <= countPlanete) {
		rf->vilocity = planets.at(planet)->getVilocity();
		rf->position = planets.at(planet)->getPos();
	} else {
		rf->vilocity = glm::vec3(0.0f);
		rf->position = glm::vec3(0.0f);
	}
	rf->scale = globalConditions->vilocityVectorLengthScale;
}

void PlaneteBuffer::drawAllVilosityArrows(ReferenceFrame* rf, int exeptFor) {
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		if (i == exeptFor)
			continue;
		planets[i]->drawVilosityVector(rf);
	}
}

Planet* PlaneteBuffer::getPlaneteByName(char* szName) {
	int countPlanete = planets.size() - 1;
	for (int i = 0; i < countPlanete; i++) {
		if (0 == strcmp(szName, planets.at(i)->name.c_str()))
			return planets.at(i);
	}
	return planets.at(0);//nullptr;
}

void PlaneteBuffer::initForceField(ForceField* FF) {
	forceFieldData.graviConst = globalConditions->gravitationalConstant;
	FF->setCostomData(&forceFieldData);
	updateForceField(FF);
}

void PlaneteBuffer::updateForceField(ForceField* FF) {
	int l = planets.size();
	glm::vec3* ps = new glm::vec3[l];
	float* m = new float[l];
	for (int i = 0; i < l; i++) {
		ps[i] = planets[i]->getPos();
		m[i] = planets[i]->getMass();
	}
	FF->setData(ps, m, l);
}