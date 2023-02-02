/*=========================================================================
Base curling Sheet
===========================================================================*/

#include "Helper.h"

BaseCurlingSheet::BaseCurlingSheet() {}

void BaseCurlingSheet::Update(int ms) {}

void BaseCurlingSheet::Draw() {}


/*==========================================================================
Long Curling Sheet
============================================================================*/

// Creating a void constructor
LongCurlingSheet::LongCurlingSheet(void) {}


// Curling sheet class member 
LongCurlingSheet::LongCurlingSheet(vec3 d, double offset_x , bool is_solid) : dim(d, offset_x)
{
	// init team in sheet.
	my_team = Team("my_team", dim, is_solid);
	other_team = Team("other_team", dim, !is_solid);

	// arrow setting
	arrow.set_start_point( this->dim.hack_circles[0].center );
	arrow.set_direction(M_PI / 2);
	arrow.set_length(0.25);

}

// Function for drawing sheet. 
void LongCurlingSheet::Draw()
{

	//sheet
	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f( this->dim.TR.elem[0], this->dim.TR.elem[1], this->dim.TR.elem[2]);
	glVertex3f( this->dim.TL.elem[0], this->dim.TL.elem[1], this->dim.TL.elem[2]);
	glVertex3f( this->dim.BL.elem[0], this->dim.BL.elem[1], this->dim.BL.elem[2]);
	glVertex3f( this->dim.BR.elem[0], this->dim.BR.elem[1], this->dim.BR.elem[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.TR.elem[0], this->dim.TR.elem[1], 0.2);
	glVertex3f(this->dim.TL.elem[0], this->dim.TL.elem[1], 0.2);
	glVertex3f(this->dim.BL.elem[0], this->dim.BL.elem[1], 0.2);
	glVertex3f(this->dim.BR.elem[0], this->dim.BR.elem[1], 0.2);
	glEnd();

	// lines
	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.TR.elem[0], this->dim.TR.elem[1], this->dim.TR.elem[2]);
	glVertex3f(this->dim.TR.elem[0], this->dim.TR.elem[1], 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.TL.elem[0], this->dim.TL.elem[1], this->dim.TL.elem[2]);
	glVertex3f(this->dim.TL.elem[0], this->dim.TL.elem[1], 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.BL.elem[0], this->dim.BL.elem[1], this->dim.BL.elem[2]);
	glVertex3f(this->dim.BL.elem[0], this->dim.BL.elem[1], 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.BR.elem[0], this->dim.BR.elem[1], this->dim.BR.elem[2]);
	glVertex3f(this->dim.BR.elem[0], this->dim.BR.elem[1], 0.2);
	glEnd();

	// hog lines
	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.hog_line_hack_start.elem[0], this->dim.hog_line_hack_start.elem[1], this->dim.hog_line_hack_start.elem[2]);
	glVertex3f( this->dim.hog_line_hack_end.elem[0], this->dim.hog_line_hack_end.elem[1], this->dim.hog_line_hack_end.elem[2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(this->dim.hog_line_house_start.elem[0], this->dim.hog_line_house_start.elem[1], this->dim.hog_line_house_start.elem[2]);
	glVertex3f(this->dim.hog_line_house_end.elem[0], this->dim.hog_line_house_end.elem[1], this->dim.hog_line_house_end.elem[2]);
	glEnd();


	// arrow 
	glBegin(GL_LINE_LOOP);
	//glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f( this->arrow.get_end_point().elem[0], this->arrow.get_end_point().elem[1], 0);
	glVertex3f(this->arrow.get_start_point().elem[0], this->arrow.get_start_point().elem[1], 0);
	glEnd();

	// circles
	for (const circle &c : this->dim.house_circles)
	{
		this->DrawCircle(c);
	}
	for (const circle &c : this->dim.hack_circles)
	{
		this->DrawCircle(c);
	}

	 //draw team A and B stones 
	for (int i = 0; i < NUM_BALLS; i++)
	{
		my_team.stones.at(i).Draw();
		other_team.stones.at(i).Draw();
	}

}

// Function for updating the game after every 10 milliseconds
void LongCurlingSheet::Update(int ms)
{
	my_team.Update(ms);
	other_team.Update(ms);

	for (int i = 0; i < NUM_BALLS; i++)
	{
		my_team.stones.at(i).Update(ms);
		other_team.stones.at(i).Update(ms);
	}

	// check collision for each ball in team A with all ball in Team A 
	// check collision fo each ball in team B with al ball in team B 
	// check collision for each ball in team A with each ball in Team B 
	
	// Lets take this for loop for team A stones 
	for (int i = 0; i < NUM_BALLS; i++) // 0, 1, 2
	{
		bool is_colliding = false;

		// This is for team A
		for (int j = 0; j < NUM_BALLS; j++) // 0, 1, 2, | 0, 1, 2, | 0, 1, 2 
		{
			if (i != j) // 0, 0 , 1 , 1 , 2, 2 
			{
				is_colliding = my_team.stones.at(i).IsCollidingWihOther(my_team.stones.at(j));
				if (is_colliding)
				{
					if (my_team.stones.at(i).is_moving)
					{
						my_team.stones.at(j).SetForce( my_team.stones.at(i).GetForce() );
						my_team.stones.at(i).SetForce( vec2(0, 0) );
						break;
					}
					if (my_team.stones.at(j).is_moving)
					{
						my_team.stones.at(i).SetForce( my_team.stones.at(j).GetForce() );
						my_team.stones.at(j).SetForce(vec2(0, 0));
						break;
					}
				}
			}
		}

		if (is_colliding) break;
	}

	// Let take this for loop for Team B stones.
	for (int i = 0; i < NUM_BALLS; i++) // 0, 1, 2
	{
		bool is_colliding = false;

		// This is for team B
		for (int j = 0; j < NUM_BALLS; j++) // 0, 1, 2, | 0, 1, 2, | 0, 1, 2 
		{
			if (i != j) // 0, 0 , 1 , 1 , 2, 2 
			{
				is_colliding = other_team.stones.at(i).IsCollidingWihOther(other_team.stones.at(j));
				if (is_colliding)
				{
					if (other_team.stones.at(i).is_moving)
					{
						other_team.stones.at(j).SetForce(other_team.stones.at(i).GetForce());
						other_team.stones.at(i).SetForce(vec2(0, 0));
						break;
					}
					if (other_team.stones.at(j).is_moving)
					{
						other_team.stones.at(i).SetForce(other_team.stones.at(j).GetForce());
						other_team.stones.at(j).SetForce(vec2(0, 0));
						break;
					}
				}
			}
		}

		if (is_colliding) break;
	}

	// Lets take this for loop for inter team stones.
	for (int i = 0; i < NUM_BALLS; i++) // 0, 1, 2
	{
		bool is_colliding = false;

		// This is for team B
		for (int j = 0; j < NUM_BALLS; j++) // 0, 1, 2, | 0, 1, 2, | 0, 1, 2 
		{

			is_colliding = my_team.stones.at(i).IsCollidingWihOther(other_team.stones.at(j));
			if (is_colliding)
			{
				if (my_team.stones.at(i).is_moving)
				{
					other_team.stones.at(j).SetForce(my_team.stones.at(i).GetForce());
					my_team.stones.at(i).SetForce(vec2(0, 0));
					break;
				}
				if (other_team.stones.at(j).is_moving)
				{
					my_team.stones.at(i).SetForce(other_team.stones.at(j).GetForce());
					other_team.stones.at(j).SetForce(vec2(0, 0));
					break;
				}
			}
			
		}

		if (is_colliding) break;
	}


}

// Function for drawing the source and target ( circles) on the sheet
void LongCurlingSheet::DrawCircle(circle c)
{
	int lineAmount = 50; //# of triangles used to draw circle

	GLfloat twicePi = 2.0f * 3.14;	//GLfloat radius = 0.8f; //radius 

	glBegin(GL_LINE_LOOP);
	
	for (int i = 0; i <= lineAmount; i++) {
		
		glVertex3f(
			c.center.elem[0] + (c.radius * cos(i * twicePi / lineAmount)),
			c.center.elem[1] + (c.radius * sin(i * twicePi / lineAmount)), 
			0.0f
		);
	}
	glEnd();
}

// Function for reseting the game
void LongCurlingSheet::Reset()
{
	for (int i = 0; i < NUM_BALLS; i++)
	{
		my_team.stones.at(i).Reset();
		other_team.stones.at(i).Reset();
	}

	arrow.reset();

}

/*==========================================================================
For Curling Stone
===========================================================================*/

// Stone class members
//int CurlingStone::stoneIndexCnt = 0;

// Creating a constructor
CurlingStone::CurlingStone(void) {}


// Constructor / special member
CurlingStone::CurlingStone(vec2 pos, vec3 minimumboundary, vec3 maximumboundary) :
	position(pos),
	init_position(pos),
	minimumboundary(minimumboundary), // class member (argument for class object)
	maximumboundary(maximumboundary)
{

}

// Function for checking if the curling stones are in the circle.
void CurlingStone::Draw(void)
{
	if (is_draw && IsHittingBoundary())
	{
		circle c = { position, radius };
		DrawStone(c);
	}
	
}


// Function for updating curling stones for 3D curling game.
void CurlingStone::Update(int ms)
{
	if (IsHittingBoundary())
	{
		// AMAZING PHYSICS HERE!!!
		vec2 resultant_force = GetForce() - friction;
		SetForce(resultant_force);
		// F = ma => a = F/m
		vec2 resultant_acceleration = resultant_force / mass;
		SetAcceleration(resultant_acceleration);
		// numerically integrate acceleration to get velocity. 
		SetVelocity(GetAcceleration() * (double)ms / 1000);
		// numerically integrate veliocity to get position.
		SetPosition(GetPosition() + GetVelocity() * (double)ms / 1000);
	}
	else
	{
		SetForce({ 0, 0 });
	}

}


// Function for drawing stones for 3D curling game.
void CurlingStone::DrawStone(circle c)
{
	int lineAmount = 50 ; //# of triangles used to draw circle

	GLfloat twicePi = 2.0f * 3.14;	//GLfloat radius = 0.8f; //radius
	glBegin(is_filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
	for (int i = 0; i <= lineAmount; i++) {
			glVertex3f(
			c.center.elem[0] + (c.radius * cos(i * twicePi / lineAmount)),
			c.center.elem[1] + (c.radius * sin(i * twicePi / lineAmount)),
			0.0f);
		}
	glEnd();
}

// function for checking if the stone is outside the y-axis boundary of the sheet.
bool CurlingStone::IsHittingBoundary()
{
	return (GetPosition().elem[0] + GetRadius() > minimumboundary.elem[0]) && 
		(GetPosition().elem[0] < maximumboundary.elem[0] + GetRadius()) && 
		(GetPosition().elem[1] + GetRadius() > minimumboundary.elem[1]) &&
		(GetPosition().elem[1] < maximumboundary.elem[1]);
}

vec2 CurlingStone::GetForce()
{
	return force; 
}

void CurlingStone::SetForce(vec2 f)
{
	if (f.elem[1] < 0) { 
		f.elem[0] = 0;
		f.elem[1] = 0; 
	}

	is_moving = f.Magnitude() > SMALL_FORCE;
	force = f;
}

vec2 CurlingStone::GetAcceleration()
{
	return acceleration;
}

void CurlingStone::SetAcceleration(vec2 acc)
{
	if (acc.elem[1] < 0 || acc.Magnitude() <= SMALL_FORCE ) { 
		acc.elem[0] = 0;
		acc.elem[1] = 0;
	}

	acceleration = acc;
}

// Get - set function for velocity of stone
vec2 CurlingStone::GetVelocity()
{
	return velocity;
}

void CurlingStone::SetVelocity(vec2 vel)
{
	if (vel.elem[1] < 0) { 
		vel.elem[0] = 0;
		vel.elem[1] = 0;
	}

	velocity = vel;
}

// Get - set function for position of the stone
vec2 CurlingStone::GetPosition()
{
	return position;
}

void CurlingStone::SetPosition(vec2 pos)
{
	position = pos;
}

// Get function for the radius of stone
float CurlingStone::GetRadius()
{
	return radius;
}

// Function to reset the stone
void CurlingStone::Reset()
{
	is_moving = false;
	is_draw = false;
	SetPosition(init_position);
	SetForce({ 0, 0 });
	SetVelocity({ 0, 0 });
	SetAcceleration({ 0, 0 });
}

// Function to check if the stone is colliding with 
//int CurlingStone::IsColliding(std::vector<CurlingStone> stones)
//{
//	int is_collide = -1;
//
//	for (int i = 0; i < stones.size(); i++)
//	{
//		if (ball_index != stones.at(i).ball_index && stones.at(i).is_draw && is_draw)
//		{
//			float distance = pow((GetPosition().elem[0] - stones.at(i).GetPosition().elem[0]), 2) + pow((GetPosition().elem[1] - stones.at(i).GetPosition().elem[1]), 2);
//			float radii = pow(radius + stones.at(i).radius, 2);
//			is_collide = distance <= radii ? i : -1;
//			if (is_collide >= 0) { break; }
//		}
//	}
//	
//
//	return is_collide;
//}

// Function to check if the stone is colliding with other stones
bool CurlingStone::IsCollidingWihOther(CurlingStone stone)
{

	if (stone.is_draw && is_draw  )
	{
		float distance = pow((GetPosition().elem[0] - stone.GetPosition().elem[0]), 2) + pow((GetPosition().elem[1] - stone.GetPosition().elem[1]), 2);
		float radii = pow(radius + stone.radius, 2);
		return distance <= radii;
	}
	else {
		return false;
	}

}

/*==========================================================================
For teams/Players and scores
===========================================================================*/

// Creating a void constructor
Team::Team() {}


// init stones for each team. 
Team::Team(std::string teamname, sheet_dim dim, bool is_solid) : name(teamname), dim(dim)
{
	// Curling stones fired from the center of source position in the direction of beam
	CurlingStone stone = CurlingStone(dim.hack_circles[0].center, dim.BL, dim.TR);
	stone.is_filled = is_solid;
	for (int i = 0; i <= NUM_BALLS; i++) { 
		stones.push_back(stone); 
	}

}


void Team::Update(int ms)
{
	for (int i = 0; i < NUM_BALLS; i++)
	{
		if (stones.at(i).is_moving)
		{
			break;
		}

	
		if (stones.at(i).is_draw && !stones.at(i).is_moving && last_ball_checked + 1 == i)
		{
			last_ball_checked = i;
			
			float stone_position_y = stones.at(i).GetPosition().elem[1];
			float house_line_1_y = this->dim.hog_line_house_start.elem[1];

			if (stone_position_y > house_line_1_y && stone_position_y <= this->dim.TL.elem[1] && stones.at(i).IsHittingBoundary())
			{
				score += 10;
			}
			else
			{
				if (!stones.at(i).IsHittingBoundary())
				{
					score -= 5;
				}
			}
			
			break;
		}

		/*if (score < 0) {
			score = 0;
		}*/
	}
}