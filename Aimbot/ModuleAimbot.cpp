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

	return true;
}
