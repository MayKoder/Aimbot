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

	origin = integrator->AddPoint(50, 500);

	target = integrator->AddPoint(800, 500);

	aimbotActive = false;

	for (int i = 0; i < MAX_PATHS; i++)
	{
		paths[i].calculated = false;
		paths[i].isValidPath = false;
		paths[i].drawColor.SetGrey();
	}
	currentItinerations = 0;
	angle = 0;

	return true;
}

// PreUpdate: clear buffer
update_status ModuleAimbot::PreUpdate()
{

	

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleAimbot::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) 
	{
		aimbotActive = true;
	}

	if (aimbotActive) 
	{
		if (currentItinerations < MAX_PATHS) 
		{
			//Do montecarlo
			float wind = 3; //m/s
			float angle_increment = 360 / (MAX_PATHS - 1);

			Point* a = new Point();
			integrator->InitPoint(a, { origin->x, origin->y });
			a->radius = 3;

			a->canCollide = false;

			integrator->AddForce(a, { 750.f * (float)COS(angle), -750.f * (float)SIN(angle) });
			for (int j = 0; j < MAX_PATH_CALC; j++)
			{
				vector2 point = { a->x, a->y };
				paths[currentItinerations].path_points.add(point);
				integrator->updateSinglePoint(a);

				if (integrator->CheckCollision(a, target))
				{
					paths[currentItinerations].drawColor.SetGreen(255);
					paths[currentItinerations].isValidPath = true;
					break;
				}
			}

			integrator->InitPoint(a, { origin->x, origin->y });
			angle += angle_increment;
			paths[currentItinerations].calculated = true;
	


			delete a;

			currentItinerations++;
		}
		else
		{
			aimbotActive = false;
			currentItinerations = 0;
			angle = 0;
		}
	}
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

// PostUpdate present buffer to screen
update_status ModuleAimbot::PostUpdate()
{




	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAimbot::CleanUp()
{
	LOG("Destroying Verlet");

	for (int i = 0; i < MAX_PATHS; i++)
	{
		paths[i].path_points.clear();
	}

	return true;
}
