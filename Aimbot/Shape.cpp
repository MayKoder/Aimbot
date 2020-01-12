#include"Shape.h"

Circle::Circle(Point* s_point, VerletIntegrator* s_integrator, Application* app)
{
	App = app;
	type = CIRCLE;
	integrator = s_integrator;
	point = s_point;

	App->verlet->integrator->world_points.add(point);
	integrator->InitPoint(point, { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() });
}

bool Shape::IsInsideMapLimits(float x, float y) 
{
	bool x_r = false;
	bool y_r = false;

	if (x != 0) 
	{
		if (x >= 10 && x <= integrator->floor_Limit_X)
		{
			return true;
		}
	}
	else
	{
		if (y >= 10 && y <= integrator->floor_Limit_Y)
		{
			return true;
		}
	}


	//10 = p-> radius, needs to be changed 
	return x_r && y_r;
}

void Line::UpdateShape() 
{

	float angle = atan2f(vertexB->y - vertexA->y, vertexB->x - vertexA->x);
	float distanceH = lenght;

	float newX = distanceH * cosf(angle);
	float newY = distanceH * sinf(angle);

	vector2 midPoint = { (vertexA->x + vertexB->x) / 2, (vertexA->y + vertexB->y) / 2 };
	vector2 corA = { vertexB->x - newX , vertexB->y - newY };
	vector2 corB = { vertexA->x + newX ,  vertexA->y + newY };
	App->renderer->DrawCircle(midPoint.x, midPoint.y, 5, 255, 255, 9, 255);


	if (!IsInsideMapLimits(midPoint.x - (newX / 2.f), 0)) 
	{
		vertexB->x = midPoint.x + (newX / 2.f);
	}
	else
	{
		vertexA->x = midPoint.x - (newX / 2.f);
		vertexB->x = midPoint.x + (newX / 2.f);
	}


	if (!IsInsideMapLimits(0, midPoint.y - (newY / 2.f)))
	{
		vertexB->y = midPoint.y + (newY / 2.f);
	}
	else
	{
		vertexA->y = midPoint.y - (newY / 2.f);
		vertexB->y = midPoint.y + (newY / 2.f);
	}

	if (!IsInsideMapLimits(midPoint.x + (newX / 2.f), 0))
	{
		vertexA->x = midPoint.x - (newX / 2.f);
	}
	else
	{
		vertexA->y = midPoint.y - (newY / 2.f);
		vertexB->y = midPoint.y + (newY / 2.f);
	}

	if (!IsInsideMapLimits(0, midPoint.y - (newY / 2.f)))
	{
		vertexB->y = midPoint.y + (newY / 2.f);
	}
	else
	{
		vertexA->y = midPoint.y - (newY / 2.f);
		vertexB->y = midPoint.y + (newY / 2.f);
	}



	//vertexA->x = midPoint.x - (newX / 2.f);
	//vertexA->y = midPoint.y - (newY / 2.f);
	//vertexB->x = midPoint.x + (newX / 2.f);
	//vertexB->y = midPoint.y + (newY / 2.f);

	//vector2 Line = { vertexA->x - vertexB->x, vertexA->y - vertexB->y };

	//vector2 SpeedA = { vertexA->vx, vertexA->vy };
	//vector2 SpeedB = { vertexB->vx, vertexB->vy };

	//float cosA = Line * SpeedA / (Line.Length() * SpeedA.Length());
	//float cosB = Line * SpeedB / (Line.Length() * SpeedB.Length());

	//float angleA = acosf(cosA);
	//float angleB = acosf(cosB);

	//float sinA = sinf(angleA);
	//float sinB = sinf(angleB);

	// vector2 ProjectedSpeedA = SpeedA * cosA;
	// vector2 ProjectedSpeedB = SpeedB * cosB;

	// vector2 NewSpeed = (ProjectedSpeedA + ProjectedSpeedB) * 0.5f;

	// SpeedA = SpeedA - ProjectedSpeedA + NewSpeed;
	// SpeedB = SpeedB - ProjectedSpeedB + NewSpeed;

	// vertexA->vx = SpeedA.x;
	// vertexA->vy = SpeedA.y;
	// vertexB->vx = SpeedB.x;
	// vertexB->vy = SpeedB.y;


	//vertexA->vx = 0;
	//vertexA->vy = 0;




}
void Line::Draw() 
{
	App->renderer->DrawLine((int)vertexA->x, (int)vertexA->y, (int)vertexB->x, (int)vertexB->y, 0, 255, 0, 255);
}

void Circle::UpdateShape()
{
	//integrator->updatePoints();
}
void Circle::Draw() 
{
	App->renderer->DrawCircle((int)point->x, (int)point->y, point->radius, 255, 255, 255, 255);
}

Box::Box(VerletIntegrator* s_integrator, Application* app)
{
	App = app;
	type = BOX;
	integrator = s_integrator;

	for (int i = 0; i < 4; i++)
	{
		box_points[i] = integrator->world_points.add(new Point())->data;
	}

	vector2 pos = { (float)App->input->GetMouseX(), (float)App->input->GetMouseY() };
	integrator->InitPoint(box_points[0], pos);
	integrator->InitPoint(box_points[1], { pos.x + long_line_length, pos.y });
	integrator->InitPoint(box_points[2], { pos.x, pos.y + short_line_length });
	integrator->InitPoint(box_points[3], { pos.x + long_line_length, pos.y + short_line_length });


	lines[0] = (Line*)integrator->shapes.add(new Line(box_points[0], box_points[1], integrator, App))->data;
	lines[1] = (Line*)integrator->shapes.add(new Line(box_points[0], box_points[2], integrator, App))->data;
	lines[2] = (Line*)integrator->shapes.add(new Line(box_points[0], box_points[3], integrator, App))->data;
	lines[3] = (Line*)integrator->shapes.add(new Line(box_points[1], box_points[3], integrator, App))->data;
	lines[4] = (Line*)integrator->shapes.add(new Line(box_points[2], box_points[3], integrator, App))->data;
	lines[5] = (Line*)integrator->shapes.add(new Line(box_points[1], box_points[2], integrator, App))->data;
}

void Box::UpdateShape()
{

	for (int i = 0; i < 6; i++)
	{

		Point* vertexA = lines[i]->vertexA;
		Point* vertexB = lines[i]->vertexB;

		if (!IsInsideMapLimits(0, vertexA->y))
		{
			vertexB->vy = 0.f;
		}
		if (!IsInsideMapLimits(0, vertexB->y))
		{
			vertexA->vy = 0.f;
		}
	}
}
void Box::Draw() {}