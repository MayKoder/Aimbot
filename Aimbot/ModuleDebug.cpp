#include "ModuleDebug.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "Module.h"
#include "p2SString.h"
#include "SDL\include\SDL.h"

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleDebug::~ModuleDebug()
{}

bool ModuleDebug::Awake()
{
	bool ret = true;
	return ret;
}

bool ModuleDebug::Start()
{
	bool ret = true;
	ret= App->fonts->Load("Fonts/debug_font.png", "APVositncelrya1234567890().,",1, 26, 38, 28);

	return ret;
}

update_status ModuleDebug::Update()
{

	//Draw particle debug info
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if (debugPointNumber + 1 >= (int)App->verlet->integrator->world_points.count())
		{
			if ((int)App->verlet->integrator->world_points.count() == 0)
				debugPointNumber = -1;
			else
				debugPointNumber = 0;
		}
		else
		{
			debugPointNumber++;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		if (debugPointNumber - 1 >= 0)
		{
			if ((int)App->verlet->integrator->world_points.count() == 0)
				debugPointNumber = -1;
			else
				debugPointNumber--;
		}
		else
		{
			debugPointNumber = (int)App->verlet->integrator->world_points.count() - 1;
		}
	}

	if (debugPointNumber >= 0) 
	{
		p2SString* pos = new p2SString("Position(%.2f,%.2f)", App->verlet->integrator->world_points[debugPointNumber]->x, App->verlet->integrator->world_points[debugPointNumber]->y);
		p2SString* vel = new p2SString("Velocity(%.2f,%.2f)", App->verlet->integrator->world_points[debugPointNumber]->vx, App->verlet->integrator->world_points[debugPointNumber]->vy);
		p2SString* accel = new p2SString("Acceleration(%.2f,%.2f)", App->verlet->integrator->world_points[debugPointNumber]->acc_x, App->verlet->integrator->world_points[debugPointNumber]->acc_y);

		App->fonts->BlitText(App->renderer->camera.x + 20, App->renderer->camera.y + 10, 1, pos->GetString());
		App->fonts->BlitText(App->renderer->camera.x + 20, App->renderer->camera.y + 60, 1, vel->GetString());
		App->fonts->BlitText(App->renderer->camera.x + 20, App->renderer->camera.y + 110, 1, accel->GetString());

		delete pos;
		delete vel;
		delete accel;
	}

	return update_status::UPDATE_CONTINUE;
}

