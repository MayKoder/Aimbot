#ifndef _SHAPES_H_
#define _SHAPES_H_

#include"IntegratorFunctions.h"
#include"ModuleVerlet.h"
#include"SDL/include/SDL.h"
#include"Application.h"

#define PI 3.14159265359f

class Application;
//Struct point declaration
struct Point 
{
	float x;
	float y;
	float old_x;
	float old_y;
	float acc_x;
	float acc_y;
	float vx;
	float vy;
	int radius = 10;
	SDL_Color color;

	//This needs to be converted to a real dt
	float dt;

	SDL_Rect selector_rect;
};

//Shape types
enum ShapeType
{
	NO_SHAPE, LINE, CIRCLE, BOX, PLANET
};


//Shape class
class Shape
{
public:

	Shape()
	{
		type = NO_SHAPE;
	}

	Shape(ShapeType s_type, VerletIntegrator* s_integrator)
	{
		type = s_type;
		integrator = s_integrator;
	}

	virtual void UpdateShape() = 0;
	virtual void Draw() = 0;
	virtual bool IsInsideMapLimits(float x, float y);

	~Shape() {}

public:

	ShapeType type;
	VerletIntegrator* integrator;
	Application* App = nullptr;

};

class VerletIntegrator;
//Line class
class Line : public Shape
{
public:

	Line(Point* pointA, Point* pointB, VerletIntegrator* s_integrator, Application* app)
	{
		type = LINE;
		App = app;
		integrator = s_integrator;
		vertexA = pointA;
		vertexB = pointB;

		Point* leftPoint;
		Point* rightPoint;

		if (vertexA->x > vertexB->x)
		{
			rightPoint = vertexA;
			leftPoint = vertexB;
		}
		else
		{
			rightPoint = vertexB;
			leftPoint = vertexA;
		}

		offsetX = (int)(rightPoint->x - leftPoint->x);
		offsetY = (int)(rightPoint->y - leftPoint->y);

		lenght = sqrtf(pow(offsetX, 2) +
			pow(offsetY, 2) * 1.0f);


	}
	~Line(){}

	void UpdateShape();
	void Draw();

public:

	//Edge
	Point* vertexA;
	Point* vertexB;

	int offsetX, offsetY;

	float lenght;

};

//Circle class
class Circle : public Shape
{
public:

	Circle(Point* s_point, VerletIntegrator* s_integrator, Application* app);
	~Circle() {}

	void UpdateShape();
	void Draw();

public:

	//Point
	Point* point;

};

//Box class
struct Point;
class Line;
class Box : public Shape
{
public:

	Box(VerletIntegrator* s_integrator, Application* app);
	~Box() {}

	void UpdateShape();
	void Draw();

public:

	//Point
	Point* box_points[4];
	Line* lines[6];
	float long_line_length = 50.f;
	float short_line_length = 50.f;

};

#endif // !_SHAPES_H
