#include "Planet.h"

Planet::Planet() {
	mass = 1.0f;
	pos = glm::vec3(0.0f);
	vilocity = glm::vec3(0.0f);
	isLight = false;
}

void Planet::setColor(glm::vec3 color) {
	setColor(glm::vec4(color.r, color.g, color.b, 1.0f));
}

void Planet::setColor(glm::vec4 col) {
	sphere.drawParam.col = col;
	trace.setColor(col);
}

glm::vec4 Planet::getColor(bool compliment) {
	if (compliment) {
		glm::vec4 c = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) - sphere.drawParam.col;
		c.a = 1.0f;
		return c;
	} else {
		return sphere.drawParam.col;
	}
}

void Planet::setAll(float Mass, float Radius, glm::vec3 col, 
	glm::vec3 initialPos, glm::vec3 initailVilocity) 
{
	setMRC(Mass, Radius, col);
	setInitial(initialPos, initailVilocity);
}

void Planet::setMRC(float Mass, float Radius, glm::vec3 col) {
	mass = Mass;
	setColor(col);
	sphere.radius = Radius;
}

void Planet::setInitial(glm::vec3 initialPos, glm::vec3 initialVilocity) {
	pos = initialPos;
	vilocity = initialVilocity;
}

Planet::~Planet() {
}

void Planet::draw() {
	sphere.draw(pos);
}

void Planet::drawVilosityVector(ReferenceFrame* rf) {
	Arrow::draw(pos, getVilosityVector(rf), getColor(true));
}

glm::vec3 Planet::getVilosityVector(ReferenceFrame* rf) {
	return rf->transform(vilocity, pos);
}

void Planet::add(PlanetePositionCalculator* pc) {
	posCalculater.calc = pc;
	posCalculater.use = true;
}

void Planet::obtainInitials(float time) {
	PlanetePositionCalculator::Vec3Pair p;
	posCalculater.calc->getPositionVilocity(&p, time);
	pos = p.x;
	vilocity = p.dxdt;
}

void Planet::addToTrace() {
	trace.add(pos);
}

void Planet::drawTrace() {
	trace.draw();
}

void Planet::respondeToFroce(glm::vec3 F, float timeMS) {
	vilocity += (timeMS / mass)*F;
	pos += timeMS*vilocity;
}

void Planet::reflectAt(glm::vec3 normalHat, float energyLoss) {
	//vilocity = vilocity - 2 * (glm::dot(vilocity, normalHat))*normalHat;
	vilocity = glm::reflect(vilocity*mass, normalHat)/mass;
	vilocity *= (1.0f - energyLoss);
}

void Planet::colideWith(Planet* p) {
	
	float toatlMass = mass + p->mass;
	
	setColor((mass*getColor() + p->getMass()*p->getColor()) / toatlMass);
	
	pos = (mass*pos + p->mass*p->pos) / toatlMass;

	vilocity = (getMomentum() + p->getMomentum()) / toatlMass; // Consider the Energy los due to the changed position with the grav Field
	mass = toatlMass;

	sphere.radius = cbrt(pow(sphere.radius, 3) + pow(p->sphere.radius, 3));

	isLight = isLightSource() || p->isLightSource();
}

float Planet::getKineticEnergy() {
	float absSq = (vilocity.x*vilocity.x + vilocity.y*vilocity.y + vilocity.z*vilocity.z);
	return 0.5f*mass*absSq;
}

void Planet::getPosAndNormalAt(float horizontal, float vertical, PositionNormalPair* pn) {
	pn->n = glm::vec3(0.0f, 1.0f, 0.0f);/*glm::vec3(cosf(vertical)*sinf(horizontal), sinf(vertical), cosf(vertical)*cosf(horizontal))*/;
	pn->p = glm::vec3(pos /*+ (pn->n * getRadius())*/);
}

void Planet::getInfo(std::string* str) {
	std::stringstream ss;
	ss << name /* << std::endl 
		<< "vec_v = <" << vilocity.x << ',' << vilocity.y << ',' << vilocity.z << '>' */
		<< std::endl << "m = " << mass << std::endl << "R = " << sphere.radius << std::endl
		/*<< "is light = " << std::boolalpha << isLightSource()*/;
	*str = ss.str();
}