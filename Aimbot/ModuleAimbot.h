#ifndef __ModuleAimbot_H__
#define __ModuleAimbot_H__

#include "Globals.h"
#include "Application.h"
#include"Module.h"
#include"Shape.h"
#include"p2List_Extended.h"


struct Point;
//Module verlet
class ModuleAimbot : public Module
{
public:

	ModuleAimbot(Application* app, bool start_enabled = true);
	~ModuleAimbot();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	void ClearWorld();
	bool CleanUp();

	VerletIntegrator* integrator;


	Point* selected_point;
	ShapeType creation_type;

	Point* MouseHoverSelection();

	bool bigPlanetEnabled = false;
	bool smallPlanetEnabled = false;

	int selector_type = 0;

};

#endif // __ModuleAimbot_H__