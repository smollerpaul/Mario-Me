#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"

#define CAM_WIDTH_SIZE	769
#define CAM_HEIGHT_SIZE	579



class GameMap;

class CPlayScene: public CScene
{
protected: 
	
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
	virtual void LoadSpecialObject(float xPos, float yPos, float width, float height, int length = 0, int type = 0, int direction=0) override;
	void CheckAlive();
	
	static const int SceneType = 776;
	virtual int GetSceneType();
};
