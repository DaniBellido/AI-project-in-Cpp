#include "stdafx.h"
#include "MyGame.h"
#include "Enemy.h"

CMyGame::CMyGame(void) :
	player(2255, 300, "player.bmp", 0), background(1280, 720, "background.bmp", 0)
	
{
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	// TODO: add the game update code here
	for (CSprite* pSprite : walls) pSprite->Update(t);

	bool bRun = IsKeyDown(SDLK_w);
	if (IsKeyDown(SDLK_UP)) player.Input(CPlayer::UP, bRun);
	else if (IsKeyDown(SDLK_DOWN)) player.Input(CPlayer::DOWN, bRun);
	else if (IsKeyDown(SDLK_LEFT)) player.Input(CPlayer::LEFT, bRun);
	else if (IsKeyDown(SDLK_RIGHT)) player.Input(CPlayer::RIGHT, bRun);
	else player.Input(CPlayer::NO_DIR, bRun);

	if (IsKeyDown(SDLK_q))
	{
		whistle.Play("whistle.wav");
		//boolean triggering enemies
	}


	//cout << "X: " << player.GetX() << " Y: " << player.GetY() << endl;

	player.Update(t);

	for (CSprite* pWall : walls) 
	{
		if (player.HitTest(pWall)) player.SetVelocity(0,0);
	}

	for (CEnemy* pEnemy : enemies)
		pEnemy->Update(t);
}

void DrawHealth(CGraphics* g, CVector pos, float w, float h, float health)
{
	if (health <= 0) return;
	g->DrawRect(CRectangle(pos.m_x - 1, pos.m_y - 1, w + 1, h + 1), CColor::Black());
	g->FillRect(CRectangle(pos.m_x, pos.m_y - 1, ceil(w * health / 100.f), h), health > 20 ? CColor::Blue() : CColor::Red());
}

void CMyGame::OnDraw(CGraphics* g)
{

	
	// TODO: add drawing code here
	CVector scroll = CVector(400, 300) - player.GetPos();
	// minimum and maximum values for the scroll - at the edges of the playfield
	if (scroll.m_x < -1280) scroll.m_x = -1280;
	if (scroll.m_y < -720) scroll.m_y = -720;
	if (scroll.m_x > 0) scroll.m_x = 0;
	if (scroll.m_y > 0) scroll.m_y = 0;

	

	// scroll and draw
	g->SetScrollPos(scroll);
	background.Draw(g);
	for (CSprite* pSprite : walls) pSprite->Draw(g);


	player.Draw(g);
	
	for (CEnemy* pEnemy : enemies)
	{
		pEnemy->Draw(g);
		DrawHealth(g, pEnemy->GetPosition() + CVector(-32, 32), 20, 4, pEnemy->GetHealth());

		g->DrawLine(pEnemy->GetPosition(), player.GetPosition(), 4, CColor::LightBlue());
	}

	DrawHealth(g, player.GetPosition() + CVector(-32, 32), 20, 4, player.GetHealth());
	for (CEnemy* pSpider : enemies)
	{
		pSpider->Draw(g);
		DrawHealth(g, pSpider->GetPosition() + CVector(-32, 32), 20, 4, pSpider->GetHealth());
	}



	if (IsGameOver())
		if (player.GetHealth() <= 0)
			*g << font(48) << color(CColor::DarkRed()) << vcenter << center << "GAME OVER" << endl;
		else
			*g << font(48) << color(CColor::DarkBlue()) << vcenter << center << "YOU ESCAPED" << endl;
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	enemies.delete_all();


	//enemies.push_back(new CEnemy(1720, 155, "enemy.bmp", &walls, 0));
	enemies.push_back(new CEnemy(2250, 590, "enemy.bmp", &walls, 0));
	//enemies.push_back(new CEnemy(715, 245, "enemy.bmp", &walls, 0));
	//enemies.push_back(new CEnemy(940, 1220, "enemy.bmp", &walls, 0));
	//enemies.push_back(new CEnemy(1650, 1015, "enemy.bmp", &walls, 0));

}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{

	CSprite* pSprite;

	for (int i = 0; i < 16; i++)
	{
		int x, y, w, h;
		//SCREEN LIMITS x,  y, w, h
		if (i == 0) { x = 20; y = 1430; w = 2560 * 2; h = 50; } //TOP WALL
		if (i == 1) { x = 20; y = 10; w = 2560 * 2; h = 50; }  //BOT WALL
		if (i == 2) { x = 0; y = 50; w = 50; h = 1440 * 2; } //LEFT WALL
		if (i == 3) { x = 2550; y = 50; w = 50; h = 1440*2; } //RIGHT WALL
		//OTHER WALLS
		if (i == 4) { x = 20; y = 1430/2; w = 2300; h = 50; } //MID WALL LEFT
		if (i == 5) { x = 2700; y = 715; w = 2300; h = 50; } //MID WALL RIGHT
		if (i == 6) { x = 1360; y = 350; w = 300; h = 440; } //BOT BLOCK
		if (i == 7) { x = 1360; y = 1090; w = 300; h = 440; } //TOP BLOCK
		if (i == 8) { x = 275; y = 390; w = 500; h = 50; } //BATH WALL
		if (i == 9) { x = 550; y = 460; w = 50; h = 200; } //BATH WALL2
		if (i == 10) { x = 2275; y = 390; w = 500; h = 50; } //STARTPOINT WALL
		if (i == 11) { x = 2000; y = 320; w = 50; h = 200; } //STARTPOINT WALL2
		if (i == 12) { x = 2100; y = 1130; w = 500; h = 50; } //topRightwalls
		if (i == 13) { x = 2100; y = 1130; w = 50; h = 260; } //topRightwalls
		if (i == 14) { x = 300; y = 1130; w = 650; h = 50; } //topLeftwalls
		if (i == 15) { x = 650; y = 930; w = 650; h = 50; } //topLeftwalls

		pSprite = new CSpriteRect(x, y, w, h, CColor::Black(), CColor::Black(), GetTime());
		pSprite->SetProperty("tag", "platform");
		walls.push_back(pSprite);

	}


}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{

}
