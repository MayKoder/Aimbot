#ifndef __ModuleVerlet_H__
#define __ModuleVerlet_H__

#include "Globals.h"
#include "Application.h"
#include"Module.h"
#include"Shape.h"
#include"p2List_Extended.h"

class VerletIntegrator;
class Shape;
enum ShapeType;
struct Point;
//Module verlet
class ModuleVerlet : public Module
{
public:

	ModuleVerlet(Application* app, bool start_enabled = true);
	~ModuleVerlet();

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

#endif // __ModuleVerlet_H__