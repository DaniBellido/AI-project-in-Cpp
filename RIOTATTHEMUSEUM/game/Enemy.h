#pragma once
//#include "MyGame.h"

class CEnemy : public CSprite
{
public:
	enum STATE { NONE, IDLE, PATROL, CHASE, SEEK, TOILET, NAP };
	//NECESITA ENUM DIR
	float PATROL_SPEED = 40;
	float CHASE_SPEED = 100;

private:
	STATE state;
	CVector playerPosition;
	float stamina;

	CSpriteList* pWalls;

public:
	CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, Uint32 time);
	CEnemy(Sint16 x, Sint16 y, CGraphics* pGraphics, Uint32 time);
	/*CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, CSpriteList* pWalls, Uint32 time);
	CEnemy(Sint16 x, Sint16 y, CGraphics* pGraphics, CSpriteList* pWalls, Uint32 time);*/
	~CEnemy(void);

	void OnUpdate(Uint32 time, Uint32 deltaTime);

	void ChangeState(STATE newState);
	STATE GetState() { return state; }

	float GetHealth() { return stamina; }

	void SetEnemyPosition(CVector v) { playerPosition = v; }
	void Attacked();
};

typedef std::_gfc_std_ext<std::list<CEnemy*>> CEnemyList;
