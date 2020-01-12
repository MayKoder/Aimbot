#ifndef __ModuleAimbot_H__
#define __ModuleAimbot_H__

#include "Globals.h"
#include "Application.h"
#include"Module.h"
#include"Shape.h"
#include"p2List_Extended.h"

#define MAX_PATH_CALC 150
#define MAX_PATHS 15

struct MonteCarlo_Path
{
	p2List_Extended<vector2> path_points;
	Color drawColor;
	bool calculated;
	bool isValidPath;
	vector2 velocity;

	void ClearPath() 
	{
		path_points.clear();
		calculated = false;
		isValidPath = false;

		drawColor.SetGrey();
		velocity.Zero();
	}
};

struct Point;
class VerletIntegrator;
//Module verlet
class ModuleAimbot : public Module
{
public:

	ModuleAimbot(Application* app, bool start_enabled = true);
	~ModuleAimbot();

	bool Init();
	update_status Update();
	bool CleanUp();

	void StartMonteCarlo();
	vector2 ExecuteMonteCarlo(Point* origin, Point* target);
	void CleanPaths();
	void CreateTargetAndOrigin();
	void ExecuteTrajectory();

	VerletIntegrator* integrator;
	
	Point* origin;
	Point* target;
	bool aimbotActive;
	int currentItinerations;
	float angle;

	MonteCarlo_Path paths[MAX_PATHS];
	MonteCarlo_Path* selected_path;

};


#endif // __ModuleAimbot_H__