#pragma once

#include <math.h>
#include <string>
#include "vec.h"

#define MAT_ZEROS		0
#define MAT_ONES		1
#define MAT_IDENTITY	2

typedef float MAT_FLOAT;

template <unsigned NUM_DIM>
class mat {
private:
	MAT_FLOAT Komponents[NUM_DIM][NUM_DIM];
public:
	mat(int Mode = MAT_ZEROS) {
		MAT_FLOAT all = Mode == MAT_ONES ? 1.0f : 0.0f;
		bool identity = Mode == MAT_IDENTITY;
		for (int i = 0; i < NUM_DIM; i++) for (int j = 0; j < NUM_DIM; j++) {
			MAT_FLOAT entry = all;
			if (identity && i == j) {
				entry = 1.0f;
			}
			Komponents[i][j] = entry;
		}
	}
	mat(MAT_FLOAT komponents[NUM_DIM][NUM_DIM]) {
		for (int i = 0; i < NUM_DIM; i++) for (int j = 0; j < NUM_DIM; j++) {
			Komponents[i][j] = komponents[i][j];
		}
	}
	mat(MAT_FLOAT komponents[NUM_DIM*NUM_DIM]) {
		int c = 0;
		for (int i = 0; i < NUM_DIM; i++) for (int j = 0; j < NUM_DIM; j++) {
			Komponents[i][j] = komponents[c];
			c++; //Yes, that is the Language
		}
	}
	~mat(){}

	MAT_FLOAT get(int i, int j) {// 0 based
		return i >= NUM_DIM && j >= NUM_DIM ? 0.0f : Komponents[i][j];
	}

	mat<NUM_DIM>& operator*(mat<NUM_DIM>& M) {
		MAT_FLOAT res[NUM_DIM][NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) for (int j = 0; j < NUM_DIM; j++) {
			MAT_FLOAT entry = 0.0f;
			for (int k = 0; k < NUM_DIM; k++)
				entry += Komponents[i][k]*M.Komponents[k][j];
			res[i][j] = entry;
		}
		return mat<NUM_DIM>(res);
	}

	void operator*=(mat<NUM_DIM>& M) {
		this = this * M;
	}

	vec<NUM_DIM>& operator*(vec<NUM_DIM>& v) {
		VEC_FLOAT res[NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) {
			VEC_FLOAT entry = 0.0f;
			for (int k = 0; k < NUM_DIM; k++)
				entry += Komponents[i][k] * v[k];
			res[i] = entry;
		}
		return vec<NUM_DIM>(res);
	}

	mat<NUM_DIM>& operator*(MAT_FLOAT t) {
		MAT_FLOAT res[NUM_DIM][NUM_DIM];
		for (int i = 0; i < NUM_DIM; i++) for (int j = 0; j < NUM_DIM; j++) {
			res[i][j] = Komponents[i][j] * t;
		}
		return mat<NUM_DIM>(res);
	}

	void rotate(float angle, vec<NUM_DIM>& axis) {

	}

	void scale(MAT_FLOAT factor) {

		this = this * mat<NUM_DIM>(MAT_IDENTITY);
	}

	void scale(MAT_FLOAT factors[NUM_DIM]) {

	}

	void translate(vec<NUM_DIM-1>& v) {

	}

	void toString(std::string* str) {
		*str += "mat";
		*str += std::to_string(NUM_DIM);
		*str += '(';
		for (int i = 0; i < NUM_DIM; i++) {
			for (int j = 0; j < NUM_DIM; j++) {
				*str += std::to_string(Komponents[i][j]);
				if (j < NUM_DIM - 1)
					*str += ',';
			}
			if(i < NUM_DIM - 1)
				*str += "\n     ";
		}
		*str += ')';
	}
};

typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;