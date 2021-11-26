#include "stdafx.h"
#include "Enemy.h"
#include "MyGame.h"
#include "Player.h"


CEnemy::CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, CSpriteList* pWalls, Uint32 time) : CSprite(x, y, 0, 0, time)
{

	LoadImage(pFileBitmap, "enemy", CSprite::Sheet(4, 1).Tile(3, 0), CColor::White());
	LoadImage(pFileBitmap, "idle", CSprite::Sheet(4, 1).Tile(0, 0), CColor::White());
	stamina = 100;
	state = NONE;
	ChangeState(IDLE);
	this->pWalls = pWalls;
}


CEnemy::~CEnemy(void)
{
}

//intersection function to test line of sight
bool Intersection(CVector a, CVector b, CVector c, CVector d, float& k1, float& k2)
{
	CVector v1 = b - a;
	CVector v2 = d - c;
	//CVector con = c - a;
	CVector con = c - a;
	float det = v1.m_x * v2.m_y - v1.m_y * v2.m_x;
	if (det != 0)
	{
		k1 = (v2.m_y * con.m_x - v2.m_x * con.m_y) / det;
		k2 = (v1.m_y * con.m_x - v1.m_x * con.m_y) / det;
		return true;
	}
	else
		return false;
}

// returns true is the line segments a-b and c-d intersect
bool Intersection(CVector a, CVector b, CVector c, CVector d)
{
	float k1, k2;
	if (!Intersection(a, b, c, d, k1, k2))
		return false;
	return k1 >= 0 && k1 <= 1.f && k2 >= 0 && k2 <= 1.f;
}

void CEnemy::OnDraw(CGraphics* g) 
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
	{
		if (rand() % 60 == 0)
			SetDirection(GetDirection() + (float)(rand() % 180 - 90));
		stamina -= 0.01f;
		// take a random turn at a random frame, on average once every 60 frames

		bObstacle = false;

		for (CSprite* pWall : *CEnemy::pWalls)
		{


			if (HitTest(pWall))
			{
				//cout << "HITTEST" << endl;
				ChangeState(IDLE);
				SetRotation(-GetRotation());
				SetVelocity(-GetVelocity());

			}
		}
		//while patrolling, the autonomous agent checks if there is any collision with any wall/obstacle

		for (CSprite* pWall : *CEnemy::pWalls)
		{
			if (Intersection(GetPosition(), playerPosition,
				CVector(pWall->GetLeft(), pWall->GetTop()),		// top-left vertex
				CVector(pWall->GetRight(), pWall->GetBottom()))) // bottom-right vertex
			{
				bObstacle = true;
				cout << bObstacle << endl;
			}
				

			if (Intersection(GetPosition(), playerPosition,
				CVector(pWall->GetLeft(), pWall->GetBottom()),		// bottom-left vertex
				CVector(pWall->GetRight(), pWall->GetTop())))		// top-right vertex
				bObstacle = true;
		}

		if (bObstacle == false)
		{
			cout << "Player Spotted" << endl;
			ChangeState(CHASE);
			break;
		}
		else
		{
			cout << "NOT FOUND" << endl;
		}
			if (bObstacle)
				break;			

		
		break;
	}
	case CHASE: 

		cout << "Enemy is chasing" << endl;
		SetDirection(playerPosition);
	
		stamina -= 0.2f;
		break;
	case SEEK:
		stamina -= 0.01f;
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
		if (enemyDistance < 50) ChangeState(SEEK);
		break;
	case PATROL:
		if (stamina < 20) ChangeState(IDLE);
		//LINE OF SIGTH CODE

		break;
	case CHASE:
		if (enemyDistance < 50) ChangeState(SEEK);
		if (enemyDistance > 250) ChangeState(IDLE);
		break;
	case SEEK:
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
	}
}
