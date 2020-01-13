#include "Globals.h"
#include "Application.h"
#include "ModuleAimbot.h"


ModuleAimbot::ModuleAimbot(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleAimbot::~ModuleAimbot()
{}

// Called before render is available
bool ModuleAimbot::Init()
{
	integrator = App->verlet->integrator;


	//Spawn origin and target in random coords
	CreateTargetAndOrigin();

	return true;
}

// PreUpdate: clear buffer
void ModuleAimbot::StartMonteCarlo()
{
	aimbotActive = true;
}

void ModuleAimbot::CleanPaths() 
{
	//Reset paths
	for (int i = 0; i < MAX_PATHS; i++)
	{
		paths[i].ClearPath();
	}
	selected_path = nullptr;
}

void ModuleAimbot::CreateTargetAndOrigin() 
{

	int h_a = rand() % ((SCREEN_HEIGHT - 50) - 50 + 1) + 50;
	int h_b = rand() % ((SCREEN_HEIGHT - 50) - 50 + 1) + 50;

	int w_a = rand() % ((300 - 100) - 100 + 1) + 100;
	int w_b = rand() % (900 - 700 + 1) + 700;
		
	origin = integrator->AddPoint(w_a, h_a);
	origin->isSimulated = false;

	target = integrator->AddPoint(w_b, h_b);
	target->isSimulated = false;

	aimbotActive = false;

	for (int i = 0; i < MAX_PATHS; i++)
	{
		paths[i].calculated = false;
		paths[i].isValidPath = false;
		paths[i].drawColor.SetGrey();
		paths[i].velocity.Zero();
	}

	currentItinerations = 0;
	angle = 0;
}

//Main aimbot func
vector2 ModuleAimbot::ExecuteMonteCarlo(Point* origin, Point* target) 
{
	vector2 ret = { 0.f, 0.f };
	if (currentItinerations < MAX_PATHS)
	{
		//Get the angle increment
		float angle_increment = 360 / (MAX_PATHS - 1);

		//Create bullet to simulate with
		Point* simulatedPoint = new Point();
		integrator->InitPoint(simulatedPoint, { origin->x, origin->y });
		simulatedPoint->radius = 3;
		simulatedPoint->canCollide = false;

		ret = { 1000.f * (float)COS(angle), 1000.f * (float)SIN(angle) };
		paths[currentItinerations].velocity = ret;

		integrator->AddForce(simulatedPoint, paths[currentItinerations].velocity);

		//Simulate bullets in angles
		for (int j = 0; j < MAX_PATH_CALC; j++)
		{
			//Simulate bullet
			vector2 point = { simulatedPoint->x, simulatedPoint->y };
			paths[currentItinerations].path_points.add(point);
			integrator->updateSinglePoint(simulatedPoint);

			//If there is a collision with the target, the current simulated path is a 
			//correct path to the target
			if (integrator->CheckCollision(simulatedPoint, target))
			{
				paths[currentItinerations].drawColor.SetGreen(255);
				paths[currentItinerations].isValidPath = true;
				break;
			}
		}

		//Increase angle and restart simulated bullet
		integrator->InitPoint(simulatedPoint, { origin->x, origin->y });
		angle += angle_increment;
		paths[currentItinerations].calculated = true;


		delete simulatedPoint;
		currentItinerations++;

	}
	else
	{
		aimbotActive = false;
		currentItinerations = 0;
		angle = 0;
	}

	return ret;

}

void ModuleAimbot::ExecuteTrajectory() 
{
	//Select short path and execute it
	for (int i = 0; i < MAX_PATHS; i++)
	{
		if (!paths[i].isValidPath)
		{
			paths[i].ClearPath();
		}
		else
		{
			if (selected_path)
			{
				//Select fastest path
				if (paths[i].path_points.count() < selected_path->path_points.count()) 
				{
					selected_path = &paths[i];
				}
			}
			else
			{
				selected_path = &paths[i];
			}
			paths[i].drawColor.SetGreen(5);
		}
	}

	//Spawn bullet with path vel
	if (selected_path) 
	{
		Point* temp_point = integrator->AddPoint(origin->x, origin->y);
		temp_point->radius = 5;
		integrator->AddForce(temp_point, { selected_path->velocity.x, selected_path->velocity.y });
	}

}

// Update: debug camera
update_status ModuleAimbot::Update()
{
	//Execute aimbot
	if (aimbotActive) 
	{
		ExecuteMonteCarlo(origin, target);
	}

	//Draw paths
	for (int i = 0; i < MAX_PATHS; i++)
	{
		if (paths[i].calculated)
		{
			for (int j = 0; j < paths[i].path_points.count(); j++)
			{
				if (paths[i].path_points[j].x != 0 && paths[i].path_points[j].y != 0)
					App->renderer->DrawCircle(paths[i].path_points[j].x, paths[i].path_points[j].y, 3, paths[i].drawColor.r, paths[i].drawColor.g, paths[i].drawColor.b, paths[i].drawColor.a);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAimbot::CleanUp()
{
	LOG("Destroying Verlet");

	//Clear paths
	for (int i = 0; i < MAX_PATHS; i++)
	{
		paths[i].path_points.clear();
	}

	return true;
}
