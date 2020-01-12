#ifndef _GRAVITATIONAL_FIELD_H_
#define _GRAVITATIONAL_FIELD_H_

#include "IntegratorFunctions.h"
#include "ModuleVerlet.h"
#include "p2List.h"
#include "IntegratorFunctions.h"
#include "Shape.h"


//calcula nueva po cogiendo actual sumando la velocidad por delta t (increment pos hacia ...)

#define G -6.67 //6.67 //blablabla ARREGLAR, MYTODO ^^
#define TIME 1

struct Point;
class Line;
class VerletIntegrator;

class Planet
{
public:

	float atmosphere = 0;
	float gravity = 0;
	float mass = 0; //planet radius
	vector2 position = { 0, 0 };
	vector2 speed = { 0, 0 };

	Planet();
};

class PlanetObject
{
public:

	vector2 speed = { 0, 0 };
	float mass = 0;
	vector2 position = { 0, 0 };

	PlanetObject();
};

//-----------------------------------------------

class GravitationalField : public Module
{
public:

	GravitationalField(Application* app, bool start_enabled = true);
	~GravitationalField();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DrawBigPlanet(Planet* Earth);
	void DrawSmallPlanet(PlanetObject* Moon);
	vector2 Move(Planet* Earth, PlanetObject* Moon);

public:
	Planet* bigPlanet;
	PlanetObject* smallPlanetObject;

	float Fg = 0;
	float Fc = 30;
	vector2 r = { 0, 0 };
	float moduleR = 0;
	vector2 Fg1, Fg2 = { 0, 0 };
	vector2 acc1, acc2 = { 0, 0 };
	vector2 speed1, speed2 = { 0, 0 };
	vector2 angAcc1, angAcc2 = { 0, 0 };

	vector2 insideAtm = { 0, 0 };
	float moduleAtm = 0;

	bool changeSign = false;
	bool leftSide = false;
	vector2 origin = { SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.5 };

	vector2 speedTest = { 0.01f, 0.f };

};


#endif // !_GRAVITATIONAL_FIELD_H_