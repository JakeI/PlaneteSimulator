#include "GlobalConditions.h"

GlobalConditions::ComandStreamPair::ComandStreamPair(std::string* initialLine) 
	: line(*initialLine)
{
	line >> command;
}

GlobalConditions::GlobalConditions() {
	headerType = UNREADABLE;
	colisionModel = COMBINE;
	gravitationalConstant = 1.0f;
	incrementsPtunit = 0.5f;
	ms2tunitFactor = 1.0f;
	renderingMethod = RenderingMethod::DIFFEQ;
	currentTime = 0.0f;
	vilocityVectorLengthScale = 1.0f;
}

GlobalConditions::~GlobalConditions() {

}

void GlobalConditions::readTillData(std::ifstream* file) {
	std::string s;
	getline(*file, s);
	setHeaderType(&s);

	getline(*file, s);
	if (s.find("'global") != std::string::npos) {

		while (getline(*file, s)) {
			if (s.find("'data") != std::string::npos)
				break;
			processDataLine(&s);
		}

	} else {
		//errorCode = 2;
	}
}

void GlobalConditions::setHeaderType(std::string* s) {
	if (s->find("#cartesian") != std::string::npos) {
		headerType = HeaderType::CARTESIAN;
	}
	else if (s->find("#kepler") != std::string::npos) {
		headerType = HeaderType::KEPLER;
	}
	else {
		headerType = HeaderType::UNREADABLE;
	}
}

void GlobalConditions::setFloat(ComandStreamPair* csp, 
	const std::string keyword, float* value)
{
	if (csp->command.find(keyword) != std::string::npos) {
		csp->line >> *value;
	}
}

void GlobalConditions::setColidionModle(ComandStreamPair* csp)
{
	if (csp->command.find("ColisionModel") != std::string::npos) {
		std::string value;
		csp->line >> value;
		if (value.find("REFLECT") != std::string::npos) {
			colisionModel = ColisionModel::REFLECT;
		} else if (value.find("COMBINE") != std::string::npos) {
			colisionModel = ColisionModel::COMBINE;
		}
	}
}

void GlobalConditions::processDataLine(std::string* s) {
	ComandStreamPair csp(s);
	setColidionModle(&csp);
	setFloat(&csp, "GravitationalConstant", &gravitationalConstant);
	setFloat(&csp, "IncrementsPerTimeUnit", &incrementsPtunit);
	setFloat(&csp, "msToTimeUnitConversionFactor", &ms2tunitFactor);
	setScales(&csp);
	setRenderingMethod(&csp);
	setFloat(&csp, "CurrentTime", &currentTime);
	setFloat(&csp, "VilocityVectorLengthScale", &vilocityVectorLengthScale);
}

float GlobalConditions::Scales::getScale(char sml) {
	return sml == 's' ? s : sml == 'm' ? m : sml == 'l' ? l : 1.0f;
}

void GlobalConditions::setScales(ComandStreamPair* csp) {
	setFloat(csp, "Scale_s", &scales.s);
	setFloat(csp, "Scale_m", &scales.m);
	setFloat(csp, "Scale_l", &scales.l);
}

void GlobalConditions::setRenderingMethod(ComandStreamPair* csp) {
	
	if (csp->command.find("RenderingMethod") != std::string::npos) {
		std::string value;
		csp->line >> value;
		if (value.find("DIFFEQ") != std::string::npos) {
			renderingMethod = RenderingMethod::DIFFEQ;
		}
		else if (value.find("KEPLERPOS") != std::string::npos) {
			renderingMethod = RenderingMethod::KEPLERPOS;
		}
	}
}