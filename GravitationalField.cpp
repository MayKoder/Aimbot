#include "IntegratorFunctions.h"
#include "ModuleVerlet.h"
#include "GravitationalField.h"
#include "Module.h"
#include "Application.h"

GravitationalField::GravitationalField(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	bigPlanet = new Planet();
	smallPlanetObject = new PlanetObject();
}

//destructor
GravitationalField::~GravitationalField()
{}

bool GravitationalField::Init()
{
	//BIG PLANET SETTING VARS
	bigPlanet->atmosphere = 150.f;
	bigPlanet->gravity = 9.806f;
	bigPlanet->mass = 50.f;
	bigPlanet->position = { SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 };
	//bigPlanet->speed = { 10.f, 10.f };
	///speed and pos = 0

	//SMALL PLANET SETTING VARS
	smallPlanetObject->mass = 10.f;
	smallPlanetObject->position = { (SCREEN_WIDTH * 0.5) + 200, (SCREEN_HEIGHT * 0.5) - 50 };
	smallPlanetObject->speed = { 0.f, 0.f };

	return true;
}

//PreUpdate:
update_status GravitationalField::PreUpdate()
{
	return UPDATE_CONTINUE;
}

//Update: debug Camera
update_status GravitationalField::Update()
{
	if (App->verlet->bigPlanetEnabled == true)
	{
		DrawBigPlanet(bigPlanet);
	}

	if (App->verlet->smallPlanetEnabled == true)
	{
		DrawSmallPlanet(smallPlanetObject);
		Move(bigPlanet, smallPlanetObject);
	}
	return UPDATE_CONTINUE;
}

//PostUpdate
update_status GravitationalField::PostUpdate()
{
	return UPDATE_CONTINUE;
}

//CleanUp
bool GravitationalField::CleanUp()
{
	delete bigPlanet;
	delete smallPlanetObject;

	return true;
}

// -------------------------------

void GravitationalField::DrawBigPlanet(Planet* Earth)
{
	App->renderer->DrawCircle(Earth->position.x, Earth->position.y, Earth->mass, 255, 255, 255, 255);
	App->renderer->DrawCircle(Earth->position.x, Earth->position.y, Earth->atmosphere, 0, 0, 255, 255);
}

void GravitationalField::DrawSmallPlanet(PlanetObject* Moon)
{
	App->renderer->DrawCircle(Moon->position.x, Moon->position.y, Moon->mass, 0, 255, 255, 255);
}

vector2 GravitationalField::Move(Planet* Earth, PlanetObject* Moon)
{
	//Calculing Fg
	//Fg = (-G * M * m)/(r * r)

	r.x = Moon->position.x - Earth->position.x;
	LOG("R.X: %f", r.x);
	r.y = Moon->position.y - Earth->position.y;
	LOG("R.Y: %f", r.y);

	moduleR = SDL_sqrt((r.x * r.x) + (r.y * r.y));
	LOG("moduleR: %f", moduleR);

	Fg = (G * Earth->mass * Moon->mass) / (moduleR * moduleR);
	LOG("Fg: %f", Fg);

	//Uncomment this code to "set" a centripetal force in the oposite direction of Fg.
	//That will variate the little's planet orbit (it'll be not a perfect orbit around the big planet).

	//Fg = Fg - Fc;

	// -------------------------------

	//Little planet touches Big Planet Surface
	insideAtm.x = Earth->position.x + Earth->mass + Moon->mass;
	insideAtm.y = Earth->position.y + Earth->mass + Moon->mass;

	moduleAtm = SDL_sqrt((insideAtm.x * insideAtm.x) + (insideAtm.y * insideAtm.y));

	//if little planet is at the right of the big planet surface
	/*if (Moon->position.x < insideAtm.x && leftSide == false)
	{
		if (leftSide == false)
		{
			Fg = 0;
			speed1 = { 0, 0 };
			speed2 = { 0, 0 };
			acc1 = { 0, 0 };
			acc2 = { 0, 0 };
		}
	}

	//if little planet is at the left of the big planet surface
	if (Moon->position.x < Earth->position.x - Earth->mass)
	{
		leftSide = true;
		insideAtm.x = Earth->position.x - Earth->mass - Moon->mass;
		//insideAtm.y = Earth->position.y + Earth->mass + Moon->mass;
		if (Moon->position.x > insideAtm.x)
		{
			Fg = 0;
			speed1 = { 0, 0 };
			speed2 = { 0, 0 };
			acc1 = { 0, 0 };
			acc2 = { 0, 0 };
		}
	}*/

	// -------------------------------

	//Apllying Fg

	//Fg apllied to satellite1
	Fg1.x = Fg * ((Moon->position.x - Earth->position.x) / moduleR);
	Fg1.y = Fg * ((Moon->position.y - Earth->position.y) / moduleR);
	//Fg apllied to satellite2
	Fg2.x = Fg * ((Earth->position.x - Moon->position.x) / moduleR);
	Fg2.y = Fg * ((Earth->position.y - Moon->position.y) / moduleR);

	LOG("Fg1x: %f, Fg1y: %f, Fg2x: %f, Fg2y: %f", Fg1.x, Fg1.y, Fg2.x, Fg2.y);

	// -------------------------------

	//Calculing Accelerations (Newton's 2nd Law)

	//Acceleration satellite1
	acc1.x = (1.f / Earth->mass) * Fg1.x;
	acc1.y = (1.f / Earth->mass) * Fg1.y;
	//Acceleration satellite2
	acc2.x = (1.f / Moon->mass) * Fg2.x;
	acc2.y = (1.f / Moon->mass) * Fg2.y;
	LOG("Acc1x: %f, Acc1y: %f, Acc2x: %f, Acc2y: %f", acc1.x, acc1.y, acc2.x, acc2.y);

	//------------------------

	//Calculing Speeds

	speed1.x = SDL_sqrt((-G * Earth->mass) / moduleR);
	speed1.y = SDL_sqrt((-G * Earth->mass) / moduleR);

	speed2.x = SDL_sqrt((-G * Earth->mass) / moduleR);
	speed2.y = SDL_sqrt((-G * Earth->mass) / moduleR);

	LOG("speed1x: %f, speed1y: %f, speed2x: %f, speed2y: %f", speed1.x, speed1.y, speed2.x, speed2.y);

	//------------------------

	//Calculing final position

	if (Moon->position.x <= SCREEN_WIDTH * 0.5 && changeSign == false)
	{
		speed1.x = speed1.x * (-1);
		speed1.y = speed1.y * (-1);
		acc1.x = acc1.x * (-1);
		acc1.y = acc1.y * (-1);

		speed2.x = speed2.x * (-1);
		speed2.y = speed2.y * (-1);
		//acc2.x = acc2.x * (-1);
		//acc2.y = acc2.y * (-1);
	}

	if (Moon->position.y >= SCREEN_HEIGHT * 0.5 && changeSign == false)
	{
		speed1.x = speed1.x * (-1);
		speed1.y = speed1.y * (-1);
		acc1.x = acc1.x * (-1);
		acc1.y = acc1.y * (-1);

		speed2.x = speed2.x * (-1);
		speed2.y = speed2.y * (-1);
		acc2.x = acc2.x * (-1);
		acc2.y = acc2.y * (-1);
	}

	Moon->position.x = Moon->position.x + ((-speed2.x / r.x) * 100 * TIME) - (0.5 * acc2.x * TIME * TIME);
	LOG("PositionX: %f", Moon->position.x);
	Moon->position.y = Moon->position.y + ((speed2.y / r.y) * 100 * TIME) - (0.5 * acc2.y * TIME * TIME);
	LOG("PositionY: %f", Moon->position.y);

	//orbitalPoints->add(&Moon->position);

	//Earth->position.x = Earth->position.x + ((speed1.x / r.x) * 100000 * TIME) - (0.5 * acc1.x * TIME * TIME);
	//LOG("PositionX: %f", Earth->position.x);
	//Earth->position.y = Earth->position.y + ((speed1.y / r.y) * 100000 * TIME) - (0.5 * acc1.y * TIME * TIME);
	//LOG("PositionY: %f", Earth->position.y);

	//Cheking if it's a perfect orbit if little planet has no speed:
	LOG("moduleR: %f", moduleR);

	return Moon->position;
}

Planet::Planet()
{}

PlanetObject::PlanetObject()
{}