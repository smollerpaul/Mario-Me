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
	CMario *player;					// A play scene has to have player, right? 
	
public: 
	CPlayScene(string id, string filePath);

	virtual void Load() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void Unload() override;

	CMario* GetPlayer();

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	virtual void LoadMapObjects(string objectName, float x, float y, float width, float height) override;
	

};
