#pragma once

class CEnemy : public CSprite
{
public:
	enum STATE { NONE, IDLE, PATROL, CHASE, SEEK};

	float PATROL_SPEED = 40;
	float CHASE_SPEED = 100;
	CVector playerPosition;
	CSpriteList* pWalls;
	bool bObstacle = false;
	CSoundPlayer sAlert;

private:
	STATE state;
	float stamina;

	

public:
	CEnemy(Sint16 x, Sint16 y, char* pFileBitmap, CSpriteList* pWalls, Uint32 time);
	CEnemy(Sint16 x, Sint16 y, CGraphics* pGraphics, CSpriteList* pWalls, Uint32 time);
	~CEnemy(void);

	void OnUpdate(Uint32 time, Uint32 deltaTime);
	virtual void OnDraw(CGraphics* g);
	void ChangeState(STATE newState);
	STATE GetState() { return state; }

	float GetHealth() { return stamina; }

	void SetPlayerPosition(CVector v) { playerPosition = v/*; cout << *//*"Player Position Updated" << endl*/; }
	
};

typedef std::_gfc_std_ext<std::list<CEnemy*>> CEnemyList;
