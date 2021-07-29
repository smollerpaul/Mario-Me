#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Utils.h"

#define CAM_WIDTH_SIZE	769
#define CAM_HEIGHT_SIZE	579

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(181, 235, 242)

class Text;
class GameMap;

class CPlayScene : public CScene
{
protected:
	CMario* player = nullptr;

public:
	float walkOutTimer = 0;
	
	CPlayScene(string id, string filePath);

	virtual void Load() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void Unload() override;

	CMario* GetPlayer();

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	virtual void LoadMapObjects(string objectName, vector< D3DXVECTOR2> cells, float x, float y, float width = 0, float height = 0) override;
	virtual void LoadSpecialObject(string objectType, vector< D3DXVECTOR2> cells, float xPos, float yPos, float width, float height, int length = 0, int type = 0, int direction = 0,
		float desX = 0, float desY = 0, int moveDir = 0, int targetReg = -1) override;
	virtual void LoadGrid(int col, int row, int cellW, int cellH) override;

	static const int SceneType = 776;
	virtual int GetSceneType();
	
};
