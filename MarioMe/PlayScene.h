#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"

#define CAM_START_X		500
#define CAM_START_Y		850

class GameMap;

class CPlayScene: public CScene
{
protected: 
	float camStartX = 0;
	float camStartY = 0;

	float camWidth = 0;
	float camHeight = 0;

	CMario* player = nullptr;
	
public: 
	CPlayScene(string id, string filePath);

	virtual void Load() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void Unload() override;

	CMario* GetPlayer();

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	virtual void LoadMapObjects(string objectName, float x, float y, float width = 0, float height = 0) override;
	void CheckAlive();
	
	static const int SceneType = 776;
	virtual int GetSceneType();
};
