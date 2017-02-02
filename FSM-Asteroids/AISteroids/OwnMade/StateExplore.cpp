#include "OwnMade\StateExplore.h"
#include "..\Ship.h"
#include "..\FSMAIControl.h"
#include "..\utility.h"
#include "..\GameSession.h"
#include <random>

StateExplore::~StateExplore()
{
}

void StateExplore::NewRandomPos()
{
	float randPosX = (rand() % 25) + 1;
	float randPosY = (rand() % 25) + 1;
	float negOrPos = rand() % 2;

	if (negOrPos == 0)
	{
		randPosX *= -1;
	}

	randPosY = (rand() % 25) + 1;
	if (negOrPos == 0)
	{
		randPosY *= -1;
	}

	
}

void StateExplore::Update(float dt)
{
	FSMAIControl* parent = (FSMAIControl*)m_parent;
	Ship*    ship = parent->m_ship;

	if (!ship)
		return;



	Point3f futurePowPosition = Point3f(randPosX, randPosY, 0);
	Point3f deltaPos = futurePowPosition - ship->m_position;
	float dist = deltaPos.Length();
	float speed = parent->m_maxSpeed;
	float time = dist / speed;
	//futurePowPosition += time*powerup->m_velocity - ship->m_velocity*time;
	Game.Clip(futurePowPosition);
	Point3f deltaFPos = futurePowPosition - ship->m_position;
	deltaFPos.Normalize();

	//add braking vec if you're going too fast
	speed = ship->m_velocity.Length();
	if (speed > parent->m_maxSpeed)
		deltaFPos += -ship->UnitVectorVelocity();

	//DOT out my velocity
	Point3f shpUnitVel = ship->UnitVectorVelocity();
	float dotVel = DOT(shpUnitVel, deltaFPos);
	float proj = 1 - dotVel;
	deltaFPos -= proj*shpUnitVel;
	deltaFPos.Normalize();

	float newDir = CALCDIR(deltaFPos);
	float angDelta = CLAMPDIR180(ship->m_angle - newDir);
	if (fabsf(angDelta) < 3 || fabsf(angDelta) > 177)//thrust
	{
		ship->StopTurn();
		if (speed < parent->m_maxSpeed || parent->m_nearestPowerupDist > 20)
			fabsf(angDelta) < 3 ? ship->ThrustOn() : ship->ThrustReverse();
		else
			ship->ThrustOff();
	}
	else if (fabsf(angDelta) <= 90)//turn front
	{
		if (angDelta > 0)
			ship->TurnRight();
		else
			ship->TurnLeft();
	}
	else//turn rear
	{
		if (angDelta < 0)
			ship->TurnRight();
		else
			ship->TurnLeft();
	}

	parent->m_target->m_position = futurePowPosition + ship->m_velocity*time;
	parent->m_targetDir = newDir;
	parent->m_debugTxt = "Explore point";
}

int StateExplore::CheckTransitions()
{
	FSMAIControl* parent = (FSMAIControl*)m_parent;

	if (parent->m_nearestPowerup &&
		parent->m_ship->GetShotLevel() < MAX_SHOT_LEVEL)
		return FSM_STATE_GETPOWERUP;

	if (parent->m_willCollide)
		return FSM_STATE_EVADE;

	return FSM_STATE_EXPLORE;
}