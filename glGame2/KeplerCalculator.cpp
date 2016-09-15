#include "KeplerCalculator.h"

KeplerCalculator::Pair::Pair() {
	value = 0.0f;
	ddt = 0.0f;
}

void KeplerCalculator::Pair::set(float Value, float Derivitiv) {
	value = Value;
	ddt = Derivitiv;
}

float KeplerCalculator::Pair::aprox(float timePassed) {
	return value + timePassed*ddt;
}

void KeplerCalculator::Pair::toRad() {
	static const float PI_180 = M_PI / 180;
	value *= PI_180;
	ddt *= PI_180;
}

void KeplerCalculator::State::set(KeplerCalculator* kc, float time) {
	if (!kc->convertedToRad) {
		kc->_I.toRad();
		kc->_L.toRad();
		kc->_wbar.toRad();
		kc->_Omega.toRad();
		kc->convertedToRad = true;
	}

	float T = time; // This might be a problem T is supposed to be a reference to the J2000.0 time Frame
	a = kc->_a.aprox(T);
	e = kc->_e.aprox(T);
	I = kc->_I.aprox(T);
	L = kc->_L.aprox(T);
	omega = kc->_wbar.aprox(T);
	Omega = kc->_Omega.aprox(T);

	if (time > 0.000001f)
		int a = 5;

	M = L - kc->_wbar.value;
	M = M + time*sqrt(1.1842402 * 332946 / pow(a, 3));
	M = M - M_2_PI * floor(M / M_2_PI);

	E = 0.0f;
}

void KeplerCalculator::State::solvekeplersEq() {
	float E = M;
	float deltaE = 1.0f;
	static const float tol = 1E-6f;
	for (int i = 0; abs(deltaE) < tol && i < 2000; i++) {
		deltaE = (E - e*sin(E) - M) / (1 - e*cos(E));
		E = E - deltaE;
	}
}

float KeplerCalculator::State::getTrueAnomaly() {
	return 2 * atan2(sqrt(1 + e)*sin(E / 2), sqrt(1 - e)*cos(E / 2));
}

float KeplerCalculator::State::getRadius() {
	return a * (1 - e*cos(E));
}

glm::mat3 KeplerCalculator::State::getZXZ() {
	return glm::mat3(
			cos(Omega)*cos(omega) - sin(Omega)*cos(I)*sin(omega),
			-cos(Omega)*sin(omega) - sin(Omega)*cos(I)*cos(omega),
			sin(Omega)*sin(I),

			sin(Omega)*cos(omega) + cos(Omega)*cos(I)*sin(omega),
			-sin(Omega)*sin(omega) + cos(Omega)*cos(I)*cos(omega),
			cos(Omega)*sin(I),
			
			sin(I)*sin(omega),
			-sin(I)*cos(omega),
			cos(I)
		);
}

KeplerCalculator::KeplerCalculator() {
	convertedToRad = false;
}

KeplerCalculator::~KeplerCalculator() {

}

void KeplerCalculator::getPositionVilocity(PlanetePositionCalculator::Vec3Pair* p, float time) {
	
	
	//getPsoitionVilocityRSchwartz(p, time);

	
	getPositionVilocityEMStandish(p, time);
	
}

void KeplerCalculator::getPositionVilocityEMStandish(PlanetePositionCalculator::Vec3Pair* p, float time) {
	
	glm::vec3 x = getPos(time);

	static const float deltaT = 8e-8;//0.00000009f;
	glm::vec3 xnext = getPos(time + deltaT);

	p->x = x;
	p->dxdt = (xnext - x) / deltaT;
}

void KeplerCalculator::getPsoitionVilocityRSchwartz(PlanetePositionCalculator::Vec3Pair* p, float time) {

	State s;
	s.set(this, time);
	s.solvekeplersEq();
	float v = s.getTrueAnomaly();
	float r_c = s.getRadius();
	glm::vec3 o = r_c * glm::vec3(cos(v), sin(v), 0);
	glm::vec3 odot = (float)(sqrt(1.1842402 * 332946 * s.a) / r_c) * 
		glm::vec3(-sin(s.E), sqrt(1-s.e*s.e)*cos(s.E), 0);
	glm::mat3 zxz = s.getZXZ();
	glm::vec3 x = zxz*o;
	glm::vec3 dxdt = zxz*odot;

	p->x = glm::vec3(x.x, x.z, x.y);
	p->dxdt = glm::vec3(dxdt.x, dxdt.z, dxdt.y);

}

glm::vec3 KeplerCalculator::getPos(float time) {
	
	// Source: "Keplerian Elements for Approximate Positions of the Major Planets" E M Standish Solar System Dynamics Group JPL / Caltech

	// Step 1
	float T = time; // This might be a problem T is supposed to be a reference to the J2000.0 time Frame
	float a = _a.aprox(T);
	float e = _e.aprox(T);
	float I = _I.aprox(T);
	float L = _L.aprox(T);
	float wbar = _wbar.aprox(T);
	float Omega = _Omega.aprox(T);

	// Step 2
	float w = wbar - Omega;
	float M = L - wbar;
	M = M - 180 * floor(M / 180);
	if (-180 > M || M > 180) {
		int a = 42;
	}

	static const float rad2deg = (180 / M_PI);
	static const float deg2rad = (M_PI / 180);

	// Step 3
	float eStar = rad2deg*e;//Why rad2deg
	float E = M + eStar*sin(deg2rad*M);
	float delataE = 1.0f;
	static const float tol = 1E-6f;
	for (int i = 0; abs(delataE) < tol && i < 2000; i++) {
		float deltaM = M - (E - eStar*sin(deg2rad*E));
		float deltaE = deltaM / (1 - e*cos(deg2rad*E));
		E = E + deltaE;
	}

	// Step 4
	float xs = a*(cos(E) - e);
	float ys = a*sqrt(1 - (e*e))*sin(E);
	float zs = 0.0f;

	// Step 5
	float x_ecl = (cos(deg2rad*w)*cos(deg2rad*Omega) - sin(deg2rad*w)*sin(deg2rad*Omega)*cos(deg2rad*I))*xs +
		(-sin(deg2rad*w)*cos(deg2rad*Omega) - cos(deg2rad*w)*sin(deg2rad*Omega)*cos(deg2rad*I))*ys;
	float y_ecl = (sin(deg2rad*w)*sin(deg2rad*I))*xs + (cos(deg2rad*w)*sin(deg2rad*I))*ys;
	float z_ecl = (cos(deg2rad*w)*sin(deg2rad*Omega) + sin(deg2rad*w)*cos(deg2rad*Omega)*cos(deg2rad*I))*xs +
		(-sin(deg2rad*w)*sin(deg2rad*Omega) + cos(deg2rad*w)*cos(deg2rad*Omega)*cos(deg2rad*I))*ys;
	
	return glm::vec3(x_ecl, y_ecl, z_ecl);

	/*// Step 6
	const float epsilon = 23.43928;

	float x_eq = x_ecl;
	float y_eq = cos(epsilon)*y_ecl - sin(epsilon)*z_ecl;
	float z_eq = cos(epsilon)*y_ecl + cos(epsilon)*z_ecl;

	return glm::vec3(x_eq, y_eq, z_eq);*/
}

void KeplerCalculator::readln(std::stringstream* ss) {
	*ss >> _a.value >> _a.ddt >> _e.value >> _e.ddt >> _I.value >> _I.ddt >> _L.value 
		>> _L.ddt >> _wbar.value >> _wbar.ddt >> _Omega.value >> _Omega.ddt;
}

/*

David Sims: 01/28/2013 12:13 CST

HYPERBOLIC ORBITS.

Let's say you want to find the heliocentric position and the sun-relative velocity of a comet in a hyperbolic orbit.

The Keplerian orbital elements for a hyperbolic orbit.
[ a, e, i, ?, ?, T ] @ time = t
Notes: a1

Sun's gravitational parameter, GM?.
GM? = 1.32712440018e20 m³ sec?²

Conversion between astronomical units and meters.
1 AU = 1.49597870691e11 meters

Hyperbolic mean motion, m, of the comet in radians per day.
m = (86400 sec/day) ?[GM?/(?a)³]

Choose time of observation, t, and convert it to Julian Date.

(For example, 1 January 2014 at 12h GMT is JD 2456659.0)

Hyperbolic mean anomaly, M, of the comet at time t.
M = m (t?T)

Eccentric anomaly, u, of the comet.
u? = 0
Repeat over index j
f? = e sinh u(j) ? u(j) ? M
f? = e cosh u(j) ? 1
f? = e sinh u(j)
f? = e cosh u(j)
d? = ?f? / f?
d? = ?f? / [ f? + ½ d? f? ]
d? = ?f? / [ f? + ½ d? f? + ? d?² f? ]
u(j+1) = u(j) + d?
Until |u(j+1)?u(j)| < 1e-12
u = u(j+1)

True anomaly, ?, of the comet.
?' = arccos { (e ? cosh u) / (e cosh u ? 1) }
if u>0 then ? = ?'
if u=0 then ? = 0
if u0 and y>0 then Q=0
if x>0 and y0 then Q=?
?' = arctan(y/x) + Q
Heliocentric longitude, ?, in degrees.
? = (180/?) ?'

Heliocentric latitude, ?', in radians.
?' = arcsin(z/r)
Heliocentric latitude, ?, in degrees.
? = (180/?) ?'

The sun-relative velocity of a hyperbolic comet in ecliptic coordinates.

Canonical velocity vector of the comet.
Vx''' = ?(a/r) ?[GM?/(?a)] sinh u
Vy''' = +(a/r) ?[GM?/(?a)] ?(e²?1) cosh u
Vz''' = 0

Rotate the canonical velocity by the argument of the perihelion.
Vx'' = Vx''' cos ? ? Vy''' sin ?
Vy'' = Vx''' sin ? + Vy''' cos ?
Vz'' = Vz'''

Rotate the double-primed vector by the inclination.
Vx' = Vx''
Vy' = Vy'' cos i
Vz' = Vy'' sin i

Rotate the single-primed vector by the longitude of ascending node to get the sun-relative velocity of the comet in ecliptic coordinates.
Vx = Vx' cos ? ? Vy' sin ?
Vy = Vx' sin ? + Vy' cos ?
Vz = Vz'

The state vector of the comet, in heliocentric ecliptic coordinates, at time t.
[ x, y, z, Vx, Vy, Vz ]

*/