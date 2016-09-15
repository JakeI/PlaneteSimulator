#include "Renderer.h"

void Renderer::ViewDimention::set(UserInput::WindowSize* windowSize) {
	width = windowSize->width;
	hight = windowSize->hight;
}

Renderer::Renderer(char* path){

	strcpy_s(INPUT_PLANETE_PATH, path);

	flyingCamera = new RotatableCamera();

	terain = new Terain();

	shader = new GameShaderProg("C:\\A\\C_Programme\\VC++\\glGame2\\glGame2\\shaders\\GameDefaultModle.vertex",
		"C:\\A\\C_Programme\\VC++\\glGame2\\glGame2\\shaders\\GameDefaultModle.fragment");

	static const float cubePosData[] = {
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

	// One color for each vertex. They were generated randomly.
	/*static const float cubeColorData[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
		};*/

	srand(clock());
	int cFloat = sizeof(cubePosData) / sizeof(float);
	float* cubeColorData = new float[cFloat];
	for (int i = 0; i < cFloat; i++) {
		cubeColorData[i] = (rand()%1000)/1000.0f;//(cubePosData[i] + 1.0f) / 2.0f;
	}

	cordSystem = new CordSystem();
	cordSystem->setOriginPos(glm::vec3(0.0f, 0.0f, 0.0f));
	cordSystem->setScale(glm::vec3(40.0f));
	cordSystem->toggleShow();
	

	cube = new ColoredModle();
	cube->setData(cFloat, (float*)cubePosData, cubeColorData);

	delete[] cubeColorData;

	plain = new Plain();
	plain->genData(64);

	planeteBuffer = new PlaneteBuffer(INPUT_PLANETE_PATH);

	viewDimention.x = 0;
	viewDimention.y = 0;

	rotation = 0.0f;

	incrementPlanets = false;
	toggleIncrementPlanets = false;

	planetSpeed = 200;

	cameraIsOnPanet = false;

	text = new txt();

	frameQuad = new FrameBufferQuad();
	planeteCamera = new OnPlaneteCamera();
	frameQuad->callUpdateScreenSize(planeteCamera);

	currentSelectionText = new txt();

	drawVilocityArrows = false;
	drawJupiterVilocityArrow = false;

	ForceFieldOn = false;
	forceField = new ForceField();
	planeteBuffer->initForceField(forceField);

	lookedAtPlanetId = 0;

	drawStarSphere = true;
	starSphere = new StarSphere(66.0f, 2000);

	drawTraces = true;
}


Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &vboColorID);
	glDeleteVertexArrays(1, &vaoTid);
	glDeleteBuffers(1, &vboTid);
	delete terain;
	delete cube;
	delete flyingCamera;
	delete shader;
	delete planeteBuffer;
	delete text;
	delete frameQuad;
	delete planeteCamera;
	delete currentSelectionText;
	delete cordSystem;
	delete forceField;
	delete starSphere;
}

void Renderer::draw() {

	LightSource ls, *ptrLs = &ls;
	int number;
	planeteBuffer->getLightSource(&number, &ptrLs);
	setLighttSources(number, ptrLs);

	frameQuad->startNewRendering();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	setCourentView(planeteCamera);

	glLineWidth(5);
	renderPlanets(planeteCamera->planeteNo);
	
	cordSystem->draw();
	if (drawStarSphere) { starSphere->draw(); }

	renderViewVector(flyingCamera);

	glDisable(GL_DEPTH_TEST);
	renderVilocityArrows(planeteCamera->planeteNo);

	glLineWidth(1);
	frameQuad->stopRendering();


	glViewport(viewDimention.x, viewDimention.y, 
		viewDimention.width, viewDimention.hight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	/*cube->prepare(true);

	glm::mat4 mModle = glm::mat4(1.0f);
	mModle = glm::rotate(mModle, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	cube->uploadMatrix(mProjection*camera->getMatrix()*mModle);
	cube->draw();

	mModle = glm::mat4(1.0f);
	mModle = glm::rotate(mModle, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	mModle = glm::translate(mModle, glm::vec3(4.0f, 0.0f, 0.0f));
	mModle = glm::rotate(mModle, rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	cube->uploadMatrix(mProjection*camera->getMatrix()*mModle);
	cube->draw();

	cube->prepare(false);*/


	/*shader->use(true);
	mModle = glm::mat4(1.0f);
	shader->uploadMatrix(mProjection*camera->getMatrix()*mModle);
	terain->draw();
	shader->use(false);*/

	/*glm::mat4 transformation(1.0f);
	transformation = glm::translate(transformation, glm::vec3(0.0f, -3.0f, 0.0f));
	transformation = glm::scale(transformation, glm::vec3(20.0f, 20.0f, 20.0f));
	//transformation = glm::rotate(transformation, (float)M_PI_4, glm::vec3(0.0f, 0.0f, 1.0f));
	plain->perpareAndDraw(timeMs, mProjection*camera->getMatrix()*transformation);*/

	setCourentView(flyingCamera);

	renderPlanets();

	renderViewVector(planeteCamera);

	glLineWidth(3);
	cordSystem->draw();
	glLineWidth(1);

	if (drawStarSphere) { starSphere->draw(); }

	renderSliders();

	renderSelctionText();

	/*text->clearText();
	*text << "F_a = <" << planeteBuffer->averageForce.force.x << ", " <<
		planeteBuffer->averageForce.force.y << ", " << planeteBuffer->averageForce.force.z << ">";
	text->setColors(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 0.4f));
	text->write(-0.95f, -0.85f, 0.04f);*/

	glDisable(GL_DEPTH_TEST);

	if (ForceFieldOn) forceField->draw();

	renderVilocityArrows(planeteCamera->planeteNo);

	setCourentView(glm::mat4(1.0f), glm::mat4(1.0f), glm::vec3(0.0f));
	renderCameraPointer();

	frameQuad->draw();

	txt::draw();
}

void  Renderer::renderVilocityArrows(int planetNo) {
	if (drawVilocityArrows) {
		ReferenceFrame rf;
		planeteBuffer->intRefFrame(&rf, -1);
		planeteBuffer->drawAllVilosityArrows(&rf);
	}
	if (drawJupiterVilocityArrow) {
		Planet* p = planeteBuffer->planets.at(lookedAtPlanetId);
		if (p != nullptr) {
			ReferenceFrame rf;
			planeteBuffer->intRefFrame(&rf, planetNo);
			glm::vec3 v = 50.0f*p->getVilosityVector(&rf);
			Arrow::draw(p->getPos(), v, p->getColor(true));
			LinearBars::output(glm::length(v), -1.0f, -0.95f, &glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)[0], -1.0f);
		}
	}
}

void Renderer::renderSliders() {
	glDisable(GL_DEPTH_TEST);

	float Energy = planeteBuffer->getTotalPotentialEnergy();
	static float factor = nanf("");
	if (isnan(factor)) {
		float l = Energy > 0 ? 0.5f : -0.5f;
		factor = l / Energy;
	}
	static const float col[] = { 1.0f, 0.0f, 0.0f };
	LinearBars::output(factor*Energy, 0.95f, 1.0f, (float*)col);

	float E_kin = planeteBuffer->getTotalKinetikEnergy();
	static float factor_kin = nanf("");
	if (isnan(factor_kin)) {
		float l = E_kin > 0 ? 0.5f : -0.5f;
		factor_kin = l / E_kin;
	}
	static const float col_kin[] = { 0.0f, 1.0f, 0.0f };
	LinearBars::output(factor_kin*E_kin, 0.9f, 0.95f, (float*)col_kin);

	static float firstEtot = 0.0f;

	float E_tot = planeteBuffer->getTotalEnergy();
	static float factor_tot = nanf("");
	if (isnan(factor_tot)) {
		float l = E_tot > 0 ? 0.5f : -0.5f;
		factor_tot = l / E_tot;
		firstEtot = factor_tot*E_tot;
	}
	static const float col_tot[] = { 1.0f, 1.0f, 0.0f };
	LinearBars::output(factor_tot*E_tot, 0.85f, 0.9f, (float*)col_tot);
}

void Renderer::renderPlanets(int exeptFor) {
	planeteBuffer->drawPlanets(exeptFor);
	if(this->drawTraces) planeteBuffer->drawPlanetTraces();
}

void Renderer::renderViewVector(RotatableCamera* viewVector) {
	Arrow::draw(viewVector->getPos(), 2.0f*viewVector->getDirection(), 
		glm::vec4(0.9f, 0.9f, 0.9f, 0.9f));
}

void Renderer::renderSelctionText() {
	currentSelectionText->clearText();
	std::string sCurrent;
	planeteBuffer->planets.at(planeteCamera->planeteNo)->getInfo(&sCurrent);
	std::string sObserved;
	planeteBuffer->planets.at(lookedAtPlanetId)->getInfo(&sObserved);
	*currentSelectionText << "==Current==" << txt::endl << sCurrent << txt::endl << txt::endl;
	if (drawJupiterVilocityArrow)
		*currentSelectionText << "==Observed==" << txt::endl << sObserved << txt::endl << txt::endl;
	*currentSelectionText << "t = " << timeMs;
	currentSelectionText->setColors(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
	currentSelectionText->write(-0.9f, 0.85f, 0.04f);
}

void Renderer::renderCameraPointer() {
	glm::vec3 p = planeteCamera->getPos() - flyingCamera->getPos();
	glm::vec3 v = flyingCamera->getDirection();
	glm::vec3 n = glm::cross(glm::cross(p, v), v);
	Arrow::draw(glm::vec3(0.85f, 0.8f, 0.0f), 0.2f*glm::normalize(n), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Renderer::updateOnPlaneteCamera() {
	planeteCamera->update(planeteBuffer);

	if (drawJupiterVilocityArrow) {
		if (lookedAtPlanetId >= planeteBuffer->planets.size()) lookedAtPlanetId = planeteBuffer->planets.size() - 1;
		Planet* p = planeteBuffer->planets.at(lookedAtPlanetId);
		if (p == nullptr)
			return;
		glm::vec3 a = p->getPos();
		glm::vec3 b = planeteBuffer->planets.at(planeteCamera->planeteNo)->getPos();
		planeteCamera->setDirection(b - a);
	}

	planeteCamera->updateMatrix();
}

void Renderer::increment(float virtualTimems, float virtualDeltaTms) {
	
	incrementAllPlanets(virtualDeltaTms);

	if (ForceFieldOn) planeteBuffer->updateForceField(forceField);
	
	updateOnPlaneteCamera();

	flyingCamera->movePlayer(virtualDeltaTms);
	flyingCamera->updateMatrix();

	rotation += spin*virtualDeltaTms;

	timeMs = virtualTimems;
}

void Renderer::incrementAllPlanets(float virtualDeltaTms) {
	if (incrementPlanets) {
		planeteBuffer->incrementPlanets(virtualDeltaTms / planetSpeed);
	} else if (toggleIncrementPlanets) {
		planeteBuffer->incrementPlanets(virtualDeltaTms / planetSpeed);
		toggleIncrementPlanets = false;
	}
}

void Renderer::updateUserInput(UserInput* userInput, INPUT_CHANGE_BUFFER changeID) {
	
	static struct OldMousCordinates { 
		bool dragging, draggingFly, draggingPlanete;  int x; int y;
		void set(UserInput::MouseCordinates* mouse) {
			x = mouse->x;
			y = mouse->y;
		}
	}oldMouseCordinates = { false, false, false, 0, 0};
	
	if (changeID & CHANGE_WINDOW_SIZE) {
		viewDimention.set(&userInput->windowSize);
		flyingCamera->updateScreenSize((RotatableCamera::ViewDimention*) &viewDimention);
		planeteCamera->updateScreenSize((RotatableCamera::ViewDimention*) &viewDimention);
	}
	if (changeID & CHANGE_KEYBOARD_STATE) {
		flyingCamera->updateDeltaPos(
				(userInput->keyboardState[(int)'A'] ? POSITION_DELTA_A : POSITION_DELTA_NONE) |
				(userInput->keyboardState[(int)'D'] ? POSITION_DELTA_D : POSITION_DELTA_NONE) |
				(userInput->keyboardState[(int)'S'] ? POSITION_DELTA_S : POSITION_DELTA_NONE) |
				(userInput->keyboardState[(int)'W'] ? POSITION_DELTA_W : POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_SHIFT] ? POSITION_DELTA_DOWN : POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_SPACE] ? POSITION_DELTA_UP : POSITION_DELTA_NONE) |
				( (userInput->keyboardState[VK_CAPITAL] || userInput->mouseButtoneStates.rightButtonPressed)
					? POSITION_DELTA_SMALL : POSITION_DELTA_NONE)
			);
		
		planeteCamera->updateDeletaPlanetePos(
				(userInput->keyboardState[VK_UP] ? PLANETE_POSITION_DELTA_UP : PLANETE_POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_DOWN] ? PLANETE_POSITION_DELTA_DOWN : PLANETE_POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_LEFT] ? PLANETE_POSITION_DELTA_LEFT : PLANETE_POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_RIGHT] ? PLANETE_POSITION_DELTA_RIGHT : PLANETE_POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_TAB] && !userInput->keyboardState[(int)'B'] && !userInput->keyboardState[VK_CONTROL]
					? PLANETE_POSITION_DELTA_PREV : PLANETE_POSITION_DELTA_NONE) |
				(userInput->keyboardState[VK_TAB] && userInput->keyboardState[(int)'B'] && !userInput->keyboardState[VK_CONTROL]
					? PLANETE_POSITION_DELTA_NEXT : PLANETE_POSITION_DELTA_NONE)
			);
		
		if (userInput->keyboardState[VK_TAB] && !userInput->keyboardState[(int)'B'] && userInput->keyboardState[VK_CONTROL])
			lookedAtPlanetId = lookedAtPlanetId + 1 < planeteBuffer->planets.size() ? lookedAtPlanetId + 1 : 0;
		if (userInput->keyboardState[VK_TAB] && userInput->keyboardState[(int)'B'] && userInput->keyboardState[VK_CONTROL])
			lookedAtPlanetId = lookedAtPlanetId - 1 > 0 ? lookedAtPlanetId - 1 : planeteBuffer->planets.size()-1;

		if (userInput->keyboardState[(int)'U']) {
			incrementPlanets = !incrementPlanets;
		}
		if (userInput->keyboardState[(int)'K']) {
			cordSystem->toggleShow();
		}
		if (userInput->keyboardState[VK_ADD]) {
			planetSpeed -= 20;
			if (planetSpeed < 20) {
				planetSpeed = 20;
			}
		}
		if (userInput->keyboardState[VK_SUBTRACT]) {
			planetSpeed += 20;
		}
		if (userInput->keyboardState[VK_F4]) { // TODO: remove controle for removed feature
			cameraIsOnPanet = !cameraIsOnPanet;
		}
		if (userInput->keyboardState[VK_F5]) {
			delete planeteBuffer;
			planeteBuffer = new PlaneteBuffer(INPUT_PLANETE_PATH);
		}
		if (userInput->keyboardState[(int)'F']) {
			toggleIncrementPlanets = true;
		}
		if (userInput->keyboardState[(int)'V']) {
			drawVilocityArrows = !drawVilocityArrows;
		}
		if (userInput->keyboardState['J']) {
			drawJupiterVilocityArrow = !drawJupiterVilocityArrow;
		}
		if (userInput->keyboardState['G']) {
			ForceFieldOn = !ForceFieldOn;
		}
		if (userInput->keyboardState['P']) {//New
			drawStarSphere = !drawStarSphere;
		}
		if (userInput->keyboardState['T']) {//New
			drawTraces = !drawTraces;
		}
		if (userInput->keyboardState[VK_NUMPAD9]) {//New
			globalDebugParam += 1;
		}
		if (userInput->keyboardState[VK_NUMPAD3]) {//New
			globalDebugParam -= 1;
		}
		
		frameQuad->changeScale(
				(userInput->keyboardState[VK_PRIOR] ? SCALE_DELTA_INCREASE : SCALE_DELTA_NONE) |	//Page up Key
				(userInput->keyboardState[VK_NEXT] ? SCALE_DELTA_DECREASE : SCALE_DELTA_NONE)		//Page up Key
			);

	}
	if (changeID & CHANGE_MOUSE_BUTTON_STATE) {
		if ((int)userInput->mouseButtoneStates.leftButtonPressed == (int)(!oldMouseCordinates.dragging))
		{// change
			oldMouseCordinates.dragging = userInput->mouseButtoneStates.leftButtonPressed;
			if (userInput->keyboardState[VK_CONTROL]) {
				oldMouseCordinates.draggingPlanete = userInput->mouseButtoneStates.leftButtonPressed;
				oldMouseCordinates.draggingFly = !userInput->mouseButtoneStates.leftButtonPressed;
			} else {
				oldMouseCordinates.draggingFly = userInput->mouseButtoneStates.leftButtonPressed;
				oldMouseCordinates.draggingPlanete = !userInput->mouseButtoneStates.leftButtonPressed;
			}
			
			oldMouseCordinates.set(&userInput->mouseCordinates);
		}
	}
	if (changeID & CHANGE_MOUSE_CORDINATES) {
		if (oldMouseCordinates.dragging) {
			RotatableCamera::MouseCoordinates oldMouse(oldMouseCordinates.x, oldMouseCordinates.y),
				newMouse(userInput->mouseCordinates.x, userInput->mouseCordinates.y);
			if (oldMouseCordinates.draggingFly) {
				flyingCamera->dragRoatate(&oldMouse, &newMouse);
			}
			else if (oldMouseCordinates.draggingPlanete) {
				planeteCamera->dragRoatate(&oldMouse, &newMouse);
			}
			oldMouseCordinates.set(&userInput->mouseCordinates);
		}
	}
	if (changeID & CHANGE_MOUSEWHEEL_STATE) {
		flyingCamera->updateFoV(userInput->mouseWheel);
		userInput->mouseWheel = 0.0f;
	}
}
