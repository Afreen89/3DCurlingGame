#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h> 
#include <vector>
#include "vecmath.h"

/*================================================================================
Macros
==================================================================================*/

//#define TABLE_X			(0.3f) //width of table from both sides 
//#define TABLE_Z			(1.2f) // length of table from both sides

#define	SIM_UPDATE_MS	(10)
#define BALL_RADIUS		(0.05f) 
#define NUM_BALLS		(5)
#define SMALL_FORCE		(0.01)


// Complex datatype for point x and y.
//struct pointf2D {
//	GLfloat x;
//	GLfloat y;
//	
//};

/*===========================================================================
For Circles (source and target) and collisions on the sheet
=============================================================================*/

// Complex datatype for starting and ending point (circles) on the sheet
struct circle {

	vec2 center;
	float radius;
	bool filled = false;

	// compute the distance between the circles, i.e. to check collision
	float distance(vec2 point)
	{
		return pow((this->center.elem[0] - point.elem[0]), 2) + pow((this->center.elem[1] - point.elem[1]), 2);
	}

	// function to check if the curling stone is in the circle 
	bool in_circle(vec2 point, float radius)
	{
		float d_square = this->distance(point);
		float r_square = pow(this->radius + radius, 2);
		return  d_square <= r_square;
	}
};

/*=========================================================================
For arrow on the sheet
=============================================================================*/

// Complex datatype defined for the beam (cue) for 3D game
struct beam {

	vec2 start_point; // center of outer circle 
	float direction;
	float length;
	int width = 3;


	// Get - set functions for start point
	vec2 get_start_point()
	{
		return this->start_point;
	}

	void set_start_point(vec2 point)
	{
		this->start_point = point;
	}

	// Get - set function for direction
	float get_direction()
	{
		return this->direction;
	}
	void set_direction(float dir)
	{
		if (dir > 3 * M_PI / 4)
		{
			this->direction = 3 * M_PI / 4;
		}
		else if (dir < M_PI / 4)
		{
			this->direction = M_PI / 4;
		}
		else
		{
			this->direction = dir;
		}
	}

	// Get - set function for length
	float get_length()
	{
		return this->length;
	}

	void set_length(float len)
	{
		if (len > 0.05 && len < 0.60)
		{
			this->length = len;
		}
	}

	//Get - set function for width
	/*void get_width()
	{
		return this -> width;
	}*/
	

	// Get - set function for end point
	vec2 get_end_point()
	{
		vec2 end_point;
		end_point.elem[0] = this->length * cos(this->get_direction()) + this->get_start_point().elem[0];
		end_point.elem[1] = this->length * sin(this->get_direction()) + this->get_start_point().elem[1];
		return end_point;
	}

	
	void reset()
	{
		this->length = 0.25;
		this->width = 2;
		this->direction = M_PI / 2;
	}
};

/*=========================================================================
For Base Curling Sheet Dimenstions
===========================================================================*/
struct sheet_dim {

	double offset_x; 
	vec3 dim; 
	vec3 TL; 
	vec3 TR; 
	vec3 BL;
	vec3 BR;

	std::vector< circle > house_circles;
	std::vector< circle > hack_circles;

	vec3 hog_line_house_start;
	vec3 hog_line_house_end;
	vec3 hog_line_hack_start;
	vec3 hog_line_hack_end;

	sheet_dim() {}
	
	sheet_dim(vec3 dim, double offset_x)
	{
		this->offset_x = offset_x;
		this->dim = dim;
		this->find_coordinates();
		this->set_circles();
		this->set_hog_lines();
	}
	
	void find_coordinates()
	{
		// find TL, TR, BL, BR;
		this->TR = vec3(1 * this->dim.elem[1] * 33 / 150 + this->offset_x, 1 * this->dim.elem[1], this->dim.elem[2]);
		this->TL = vec3(-1 * this->dim.elem[1] * 33/150 + this->offset_x,  1*	this->dim.elem[1], this->dim.elem[2]);
		this->BL = vec3(-1 * this->dim.elem[1] * 33 / 150 + this->offset_x, -1*	this->dim.elem[1], this->dim.elem[2]);
		this->BR = vec3( 1 * this->dim.elem[1] * 33 / 150 + this->offset_x, -1*	this->dim.elem[1], this->dim.elem[2]);
	}

	void set_hog_lines()
	{
		hog_line_house_start = vec3(
			this->TR.elem[0],
			this->TR.elem[1] * 36.0f/75.0f,
			this->TR.elem[2]
		);

		hog_line_house_end = vec3(
			this->TL.elem[0],
			this->TL.elem[1] * 36.0f / 75.0f,
			this->TL.elem[2]
		);

		hog_line_hack_start = vec3(
			this->BR.elem[0],
			this->BR.elem[1] * 36.0f / 75.0f,
			this->BR.elem[2]
		);

		hog_line_hack_end = vec3(
			this->BL.elem[0],
			this->BL.elem[1] * 36.0f / 75.0f,
			this->BR.elem[2]
		);

	}

	void set_circles()
	{
		double x1 = (this->TR.elem[0] + this->TL.elem[0]) / 2;
		double y1 = (this->TR.elem[1]) * 57.0f / 75.0f;
		vec2 house_center = vec2(x1, y1);

		double x2 = (this->BR.elem[0] + this->BL.elem[0]) / 2;
		double y2 = (this->BR.elem[1]) * 57.0f / 75.0f;
		vec2 hack_center = vec2(x2, y2);


		int count = 4;
		this->house_circles.resize(count);
		this->hack_circles.resize(count);

		double radii[] = { this->TR.elem[1] * 1.0/150 , this->TR.elem[1] * 4 / 150, this->TR.elem[1] * 8 / 150, this->TR.elem[1] * 12 / 150 };
		
		for (int i = 0; i < count; i++)
		{

			this->house_circles.at(i).filled = i  < 2;
			this->house_circles.at(i).radius = radii[i];
			this->house_circles.at(i).center = house_center;

			this->hack_circles.at(i).filled = i < 2;
			this->hack_circles.at(i).radius = radii[i];
			this->hack_circles.at(i).center = hack_center; 
		}

	}

};


/*=========================================================================
For Base Curling Sheet
===========================================================================*/

class BaseCurlingSheet
{
public:
	BaseCurlingSheet();

	
	virtual void Update(int ms);
	virtual void Draw();


private:


protected:

};


/*==========================================================================
For curling stones 
============================================================================*/

// Task 2. Curling Stone class: Here are all the variables and functions declarations for the Curling Stone class

class CurlingStone
{

public:
	CurlingStone(void);
	CurlingStone(vec2 pos, vec3 minimumboundary, vec3 maximumboundary);

	
	//int IsColliding(std::vector<CurlingStone> stones);

	vec3 minimumboundary;
	vec3 maximumboundary;
	vec2 position = vec2(0, 0);
	vec2 velocity = vec2(0, 0);
	vec2 acceleration = vec2(0, 0);
	vec2 force = vec2(0, 0);
	vec2 init_position = vec2(0, 0);
	vec2 friction = vec2(0, 0.1);

	float radius = 0.025;
	float mass = 1;

	bool is_moving = false;
	bool is_draw = false;
	bool is_filled = false;
	bool is_rendered = false;

	vec2 original_force_applied = vec2(0.0f, 0.0f);

	//static int stoneIndexCnt;
	bool IsCollidingWihOther(CurlingStone stone);
	bool IsHittingBoundary();


	void Draw();
	void Update(int ms);
	void Reset();
	void DrawStone(circle c);
	void SetPosition(vec2 pos);
	void SetVelocity(vec2 vel);
	void SetAcceleration(vec2 acc); 
	void SetForce(vec2 f);
	float GetRadius();

	vec2 GetPosition();
	vec2 GetVelocity();
	vec2 GetAcceleration();
	vec2 GetForce();


};

/*==========================================================================
For teams/Players and scores
===========================================================================*/ 

// Task 3. Class for teams, players and scores

class Team {

public:
	Team();
	Team(std::string teamname, sheet_dim dim, bool is_solid);

	void Update(int ms);
	vec3 hog_line_house_start;
	vec3 hog_line_house_end;

	std::string name;
	int score = 0;

	sheet_dim dim;
	int last_ball_checked = -1;

	std::vector<CurlingStone> stones;

	
private:
	std::vector<std::string> players;

};

/*=========================================================================	
For one curling sheet
=================================================================== ==========*/

// Task 1. Curling Sheet class: Here are all the variables and functions declarations for the Curling Sheet class.

class LongCurlingSheet :
	public BaseCurlingSheet
{
public:
	LongCurlingSheet(void);
	LongCurlingSheet(vec3 d, double offset_x, bool is_solid );
	

	sheet_dim dim;
	beam arrow;

	Team my_team;
	Team other_team;
	

	void Update();
	void Update(int duration);
	void Draw();
	void Reset();

	void DrawCircle(circle c);

private:



};
