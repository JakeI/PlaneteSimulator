#pragma once

#include <sstream>
#include <fstream>

class GlobalConditions {
private:

	struct ComandStreamPair {
		std::stringstream line;
		std::string command;
		ComandStreamPair(std::string* initialLine);
	};

	void setFloat(ComandStreamPair* csp, const std::string keyword, float* value);
	void setColidionModle(ComandStreamPair* csp);
	void setScales(ComandStreamPair* csp);
	void setRenderingMethod(ComandStreamPair* csp);
	
	void setHeaderType(std::string* s);

	void processDataLine(std::string* s);	

public:

	enum HeaderType {
		UNREADABLE,
		CARTESIAN,
		KEPLER
	} headerType;

	enum ColisionModel { 
		COMBINE,
		REFLECT
	} colisionModel;

	float gravitationalConstant;
	float incrementsPtunit;
	float ms2tunitFactor;

	struct Scales {
		float s, m, l;
		Scales() { s = 1.0f; m = 1.0f; l = 1.0f; }
		float getScale(char sml);
	} scales;

	enum RenderingMethod {
		DIFFEQ,
		KEPLERPOS
	} renderingMethod;

	float currentTime;
	
	float vilocityVectorLengthScale;

	GlobalConditions();
	~GlobalConditions();

	void readTillData(std::ifstream* file);

};