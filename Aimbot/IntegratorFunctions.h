#ifndef _INTEGRATORFUNCTIONS_H_
#define _INTEGRATORFUNCTIONS_H_

#include "p2List.h"
#include"p2List_Extended.h"
#include"IntegratorFunctions.h"
#include"Shape.h"
#include"Application.h"

struct Point;
class Line;
class Application;
class Shape;
class VerletIntegrator 
{
public:
	VerletIntegrator();
	VerletIntegrator(Application* app);

	//Init a point with default variables
	void InitPoint(Point* p, vector2 pos);


	//Update point position
	void updatePoints();


	void AddForce(Point* p, vector2 force);
	void OnCollision(Point* p, Point* check_Point);
	Application* App;


public:

	//All shapes in the world
	p2List_Extended<Shape*> shapes;

	//All points in the world
	p2List_Extended<Point*> world_points;


	//p2List<Point> point;
	float bounce = 0.9f;
	//float gravity = 9.81f;
	float gravity = 500.f;
	float friction = 30.f;

	int floor_Limit_Y = SCREEN_HEIGHT - 10;
	int floor_Limit_X = SCREEN_WIDTH-10;

};

#endif // !_INTEGRATORFUNCTIONS_H_
