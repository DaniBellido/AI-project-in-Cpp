#include "stdafx.h"
#include "Enemy.h"



//CEnemy::CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, CSpriteList* pWalls, Uint32 time) : CSprite(x, y, 0, 0, time)
//{
//
//	LoadImage(pFileBitmap, "enemy", CSprite::Sheet(4, 1).Tile(3, 0), CColor::White());
//	LoadImage(pFileBitmap, "idle", CSprite::Sheet(4, 1).Tile(0, 0), CColor::White());
//	stamina = 100;
//	state = NONE;
//	ChangeState(IDLE);
//	this->pWalls = pWalls;
//}

CEnemy::CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, Uint32 time) : CSprite(x, y, 0, 0, time)
{

	LoadImage(pFileBitmap, "enemy", CSprite::Sheet(4, 1).Tile(3, 0), CColor::White());
	LoadImage(pFileBitmap, "idle", CSprite::Sheet(4, 1).Tile(0, 0), CColor::White());
	stamina = 100;
	state = NONE;
	ChangeState(IDLE);
	/*this->pWalls = pWalls;*/
}

CEnemy::~CEnemy(void)
{
}



void CEnemy::OnUpdate(Uint32 time, Uint32 deltaTime)
{
	// State-dependent actions
	switch (state)
	{
	case IDLE:
		if (stamina < 100) stamina += 0.5f;
		break;
	case PATROL:
		if (rand() % 60 == 0)
			SetDirection(GetDirection() + (float)(rand() % 180 - 90));
		stamina -= 0.01f;
		// take a random turn at a random frame, on average once every 60 frames
		break;
	case CHASE: 
		SetDirection(playerPosition - GetPosition());
		stamina -= 0.2f;
		break;
	case SEEK:
		stamina -= 0.01f;
		break;
	case TOILET:
		SetDirection(GetPosition() - playerPosition);
		stamina -= 0.01f;
		break;
	case NAP:
		break;
	}

	// Generic behaviour: bounce from the screen margins
	if (GetX() < 32 || GetX() > 2560 - 32) SetVelocity(Reflect(GetVelocity(), CVector(1, 0)));
	if (GetY() < 32 || GetY() > 1440 - 32) SetVelocity(Reflect(GetVelocity(), CVector(0, 1)));



	// if in motion, rotate so that to follow the direction of the motion; otherwise reset rotation
	if (GetSpeed() > 0)
		SetRotation(GetDirection() - 90);
	else if (state != SEEK)
		SetRotation(0);

	// Transitions
	float enemyDistance = Distance(playerPosition, GetPosition());
	switch (state)
	{
	case IDLE:
		if (stamina > 95) ChangeState(PATROL);
		if (enemyDistance < 200 && stamina > 40) ChangeState(CHASE);
		if (enemyDistance < 50) ChangeState(SEEK);
		if (stamina < 1) ChangeState(NAP);
		break;
	case PATROL:
		if (enemyDistance < 200) ChangeState(CHASE); //line of sight needed
		if (stamina < 20) ChangeState(IDLE);


		break;
	case CHASE:
		if (enemyDistance < 50) ChangeState(SEEK);
		if (enemyDistance > 250) ChangeState(IDLE);
		if (stamina < 30) ChangeState(TOILET);
		break;
	case SEEK:
		if (enemyDistance > 64) ChangeState(CHASE); //line of sight needed
		if (stamina < 30) ChangeState(TOILET); //pathfinding needed
		if (stamina < 1) ChangeState(NAP);
		break;
	case TOILET:
		if (stamina < 10) ChangeState(IDLE);
		if (enemyDistance > 250) ChangeState(IDLE);
		break;
	case NAP:
		break;
	}

	CSprite::OnUpdate(time, deltaTime);
}

void CEnemy::ChangeState(STATE newState)
{
	if (newState == state)
		return;		// No state change

	state = newState;

	switch (state)
	{
	case IDLE:
		SetVelocity(0, 0);
		SetImage("idle");
		break;
	case PATROL:
		SetDirection((float)(rand() % 360));
		SetSpeed(PATROL_SPEED);
		SetImage("enemy");
		break;
	case CHASE:
		SetDirection(playerPosition - GetPosition());
		SetSpeed(CHASE_SPEED);
		SetImage("enemy");
		break;
	case SEEK:
		SetVelocity(0, 0);
		SetImage("enemy");
		break;
	case TOILET:
		SetDirection(GetPosition() - playerPosition);
		SetSpeed(CHASE_SPEED);
		SetImage("enemy");
		break;
	case NAP:
		SetVelocity(0, 0);
		SetImage("enemy");
		break;
	}
}
