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
	bool CleanUp();

	VerletIntegrator* integrator;

};

#endif // __ModuleAimbot_H__