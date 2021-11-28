#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(Sint16 x, Sint16 y, char* pFileBitmap, Uint32 time) : CSprite(x, y, 0, 0, time)
{
	
	LoadImage(pFileBitmap, "idleR", CSprite::Sheet(4, 1).Tile(3, 0), CColor::White());
	LoadImage(pFileBitmap, "idleD", CSprite::Sheet(4, 1).Tile(1, 0), CColor::White());
	LoadImage(pFileBitmap, "idleL", CSprite::Sheet(4, 1).Tile(2, 0), CColor::White());
	LoadImage(pFileBitmap, "idleU", CSprite::Sheet(4, 1).Tile(0, 0), CColor::White());

	health = 100;
	state = NONE;
	ChangeState(IDLE, DOWN);
}



CPlayer::~CPlayer(void)
{
}

void CPlayer::OnAttacked()
{
	health -= 1.0f;
	if (health < 0) health = 0;
}

void CPlayer::OnUpdate(Uint32 time, Uint32 deltaTime)
{
	CSprite::OnUpdate(time, deltaTime);
	if (GetState() == RUN) 
	{
		health -= 0.2f;
	}

	if (GetState() == WALK && health <=100)
	{
		health += 0.2f;
	}

	if (GetState() == IDLE && health <= 100)
	{
		health += 0.5f;
	}

}

void CPlayer::ChangeState(STATE newState, DIR newDir)
{
	if (newState == state && newDir == dir)
		return;		// no state change
	if (newState == state && newDir == NO_DIR)
		return;		// also no state change

	state = newState;
	if (newDir != NO_DIR)
		dir = newDir;

	switch (state)
	{
	case WALK:
		switch (dir)
		{
		case UP: SetImage("idleU"); SetVelocity(0, WALK_SPEED); break;
		case DOWN: SetImage("idleD"); SetVelocity(0, -WALK_SPEED); break;
		case LEFT: SetImage("idleL"); SetVelocity(-WALK_SPEED, 0); break;
		case RIGHT: SetImage("idleR"); SetVelocity(WALK_SPEED, 0); break;
		}
		break;

	case RUN:
		switch (dir)
		{
		case UP: SetImage("idleU"); SetVelocity(0, RUN_SPEED); break;
		case DOWN: SetImage("idleD"); SetVelocity(0, -RUN_SPEED); break;
		case LEFT: SetImage("idleL"); SetVelocity(-RUN_SPEED, 0); break;
		case RIGHT: SetImage("idleR"); SetVelocity(RUN_SPEED, 0); break;
		}
		break;

	case TIPTOE:
		switch (dir)
		{
		case UP: SetImage("idleU"); SetVelocity(0, TIPTOE_SPEED); break;
		case DOWN: SetImage("idleD"); SetVelocity(0, -TIPTOE_SPEED); break;
		case LEFT: SetImage("idleL"); SetVelocity(-TIPTOE_SPEED, 0); break;
		case RIGHT: SetImage("idleR"); SetVelocity(TIPTOE_SPEED, 0); break;
		}
		break;

	case IDLE:
		SetVelocity(0, 0);
		switch (dir)
		{
		case UP: SetImage("idleU"); break;
		case DOWN: SetImage("idleD"); break;
		case LEFT: SetImage("idleL"); break;
		case RIGHT: SetImage("idleR"); break;
		}
		break;
	}
}

void CPlayer::Input(DIR newDir, bool bRun)
{
	STATE newState;
	if (bRun)
		newState = RUN;
	else if (newDir != NO_DIR)
		newState = WALK;
	else
		newState = IDLE;

	ChangeState(newState, newDir);
}


