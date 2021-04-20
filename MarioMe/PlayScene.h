#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"

class GameMap;

class CPlayScene: public CScene
{
protected: 
	GameMap* map = NULL;
	CMario *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_OBJECTS(string line);

	
public: 
	CPlayScene(string id, string filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

};
