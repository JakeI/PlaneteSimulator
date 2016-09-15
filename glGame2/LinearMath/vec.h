#pragma once

#include <math.h>
#include <string>

#define VEC_X			0
#define VEC_Y			1
#define VEC_Z			2
#define VEC_W			3

#define VEC_R			VEC_X
#define VEC_G			VEC_Y
#define VEC_B			VEC_Z
#define VEC_A			VEC_W

#define VEC_U			VEC_X
#define VEC_V			VEC_Y
#define VEC_S			VEC_Z
#define VEC_T			VEC_W

#define VEC_ZEROS		0
#define VEC_ONES		1
#define VEC_INDICIES	2

#define VEC_DEFAULT_EPSILON		0.000001f
#define VEC_DEFAULT_MAX_R_SQ	0.000001f

typedef float VEC_FLOAT;

template <unsigned NUM_DIM>
class vec {
private:
	VEC_FLOAT Komponents[NUM_DIM];
public:
	vec() {
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] = 0.0f;
		}
	}
	vec(VEC_FLOAT komponents[NUM_DIM]) {
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] = komponents[i];
		}
	}
	vec(VEC_FLOAT x, VEC_FLOAT y) {
		Komponents[0] = x;
		Komponents[1] = y;
		switch (NUM_DIM) {
			case 3:
				Komponents[2] = 0.0f;
			case 4:		
				Komponents[3] = 1.0f;
		}
	}
	vec(VEC_FLOAT x, VEC_FLOAT y, VEC_FLOAT z) {
		Komponents[0] = x;
		Komponents[1] = y;
		Komponents[2] = z;
		if (NUM_DIM == 4) {
			Komponents[3] = 1.0f;
		}
	}
	vec(VEC_FLOAT x, VEC_FLOAT y, VEC_FLOAT z, VEC_FLOAT w) {
		Komponents[0] = x;
		Komponents[1] = y;
		Komponents[2] = z;
		Komponents[3] = w;
	}
	~vec() {
	}

	float get(int komponent) {//0 based
		if (komponent > NUM_DIM - 1)
			return 0.0f;
		return operator[](komponent);
	}
	float operator[](int komponent) {//0 base
		return Komponents[komponent];
	}

	void operator+=(vec<NUM_DIM> v) {
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] += v.Komponents[i];
		}
	}
	void operator-=(vec<NUM_DIM> v) {
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] -= v.Komponents[i];
		}
	}
	void operator*=(VEC_FLOAT t) {
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] *= t;
		}
	}

	vec<NUM_DIM>& copy() {
		vec<NUM_DIM> result(Komponents);
		return result;
	}

	vec<NUM_DIM>& norm() {
		vec<NUM_DIM> result(Komponents);
		result.normSelf();
		return result;
	}
	void normSelf() {
		VEC_FLOAT absolute = abs();
		for (int i = 0; i < NUM_DIM; i++) {
			Komponents[i] /= absolute;
		}
	}

	float abs() {
		return sqrt(absSq());
	}
	float absSq() {
		VEC_FLOAT result = 0.0f;
		for (int i = 0; i < NUM_DIM; i++) {
			result += Komponents[i] * Komponents[i];
		}
		return result;
	}

	VEC_FLOAT dot(vec<NUM_DIM> v) {
		VEC_FLOAT result = 0.0f;
		for (int i = 0; i < NUM_DIM; i++) {
			result += Komponents[i] * v.Komponents[i];
		}
		return result;
	}

	vec<NUM_DIM>& operator+(vec<NUM_DIM>& v) {
		VEC_FLOAT result[NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) {
			result[i] = Komponents[i] + v.Komponents[i];
		}
		return vec<NUM_DIM>(result);
	}
	vec<NUM_DIM>& operator-(vec<NUM_DIM>& v) {
		VEC_FLOAT result[NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) {
			result[i] = Komponents[i] - v.Komponents[i];
		}
		return vec<NUM_DIM>(result);
	}
	vec<NUM_DIM>& operator*(VEC_FLOAT t) {
		VEC_FLOAT result[NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) {
			result[i] = Komponents[i] + t;
		}
		return vec<NUM_DIM>(result);
	}

	bool operator==(vec<NUM_DIM>& v) {
		return eq(v);
	}
	bool eq(vec<NUM_DIM>& v, VEC_FLOAT epsilon = VEC_DEFAULT_EPSILON) {
		bool result = true;
		for (int i = 0; i < NUM_DIM; i++) {
			result = result && Komponents[i] < v.Komponents[i] + epsilon
				&& Komponents[i] > v.Komponents[i] - epsilon;
			if (!result)
				break;
		}
		return result;
	}
	bool eqCircle(vec<NUM_DIM>& v, VEC_FLOAT maxRsq = VEC_DEFAULT_MAX_R_SQ) {
		return maxRsq >(this - v).absSq();
	}

	void toString(std::string* str) {
		*str += "vec";
		*str += std::to_string(NUM_DIM);
		*str += '(';
		*str += std::to_string(Komponents[0]);
		for (int i = 1; i < NUM_DIM; i++) {
			*str += ',';
			*str += std::to_string(Komponents[i]);
		}
		*str += ')';
	}
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;