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

	App->verlet->integrator->updatePoints();
	//TODO: add lists with [] operator
	for (unsigned int i = 0; i < integrator->shapes.count(); i++)
	{
		integrator->shapes[i]->UpdateShape();
	}

	if (selected_point)
	{
		if (selector_type == 1)
		{
			App->renderer->DrawLine(App->input->GetMouseX(), App->input->GetMouseY(), (int)selected_point->x, (int)selected_point->y, 0, 255, 0);
		}
		else
		{
			App->renderer->DrawLine(App->input->GetMouseX(), App->input->GetMouseY(), (int)selected_point->x, (int)selected_point->y, 255, 255, 0);
		}
	}

	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleAimbot::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		switch (creation_type)
		{
		case NO_SHAPE:

			break;

		case CIRCLE:
			integrator->shapes.add(new Circle(new Point(), integrator, App));
			if (App->debug->debugPointNumber == -1)
				App->debug->debugPointNumber = 0;
			break;
		case BOX:
			integrator->shapes.add(new Box(integrator, App));
			if (App->debug->debugPointNumber == -1)
				App->debug->debugPointNumber = 0;
			break;
		}

	}


	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		Point* sel = MouseHoverSelection();
		if (sel)
		{
			selected_point = sel;
			selector_type = 1;
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP && selected_point)
	{
		Point* sel = MouseHoverSelection();
		if (sel && sel != selected_point)
		{
			integrator->shapes.add(new Line(selected_point, sel, integrator, App));
		}
		selected_point = nullptr;
	}

	//Check for selection
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		Point* sel = MouseHoverSelection();
		if (sel)
		{
			selected_point = sel;
			selector_type = 2;
		}
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && selected_point)
	{

		//int x = App->input->GetMouseX() - p.old_x;
		//int y = App->input->GetMouseY() - p.old_y;		
		float x = (float)App->input->GetMouseX() - selected_point->x;
		float y = (float)App->input->GetMouseY() - selected_point->y;

		integrator->AddForce(selected_point, { x * 3, y * 3 });

		selected_point = nullptr;

	}





	//Draw launch line
	for (unsigned int i = 0; i < integrator->world_points.count(); i++)
	{
		Point* tmp_point = integrator->world_points[i];
		App->renderer->DrawCircle((int)tmp_point->x, (int)tmp_point->y, tmp_point->radius, 255, 255, 255, 255);
		tmp_point->selector_rect.x = (int)tmp_point->x - tmp_point->selector_rect.w / 2;
		tmp_point->selector_rect.y = (int)tmp_point->y - tmp_point->selector_rect.h / 2;
		if (tmp_point == integrator->world_points[App->debug->debugPointNumber])
			App->renderer->DrawQuad({ (int)tmp_point->selector_rect.x, (int)tmp_point->selector_rect.y, 20, 20 }, (int)tmp_point->color.r, (int)tmp_point->color.g, (int)tmp_point->color.b, (int)tmp_point->color.a);
	}
	for (unsigned int i = 0; i < integrator->shapes.count(); i++)
	{
		Shape* tmp = integrator->shapes[i];
		tmp->Draw();
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

	ClearWorld();

	delete integrator;

	return true;
}
