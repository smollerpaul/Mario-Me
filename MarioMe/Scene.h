#pragma once
#include "Utils.h"

using namespace std;

class Camera;
class CGameObject;
class GameMap;
class Grid;

class CScene
{
protected:
	string id;
	string sceneFilePath;

	Camera* camera;
	vector<CGameObject*> objects;
	GameMap* map = NULL;
	Grid* grid = nullptr;

public: 
	int pSwitchActivated = 0;

	int showTextStart = 0;
	int marioWalkStart = 0;
	int textShown = 0;

	int doneTitle = 0;
	int marioFinishLevel = 0;

	CScene();
	CScene(string id, string filePath);
	virtual void LoadMapObjects(string objectName, vector< D3DXVECTOR2> cells, float x, float y, float width, float height);
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	virtual void LoadMapEntries(float xPos, float yPos, int nodeID, string sceneID,
		string checkedSprite, string uncheckedSprite,
		string adjList, string adjWeight, int worldNumber, bool isStartPos);

	virtual void LoadBackground(float xPos, float yPos);
	virtual void LoadSpecialObject(string objectType, vector< D3DXVECTOR2> cells, float xPos, float yPos, float width, float height, int length = 0, int type = 0, int direction = 0,
		float desX = 0, float desY = 0, int moveDir = 0, int targetReg = 0);

	Camera* GetCamera();
	GameMap* GetMap();

	void SetCamera(Camera* camera);
	void AddObject(CGameObject* obj);
	void RemoveObject();
	string GetSceneId();
	
	virtual void LoadGrid(int col, int row, int cellW, int cellH);
	Grid* GetGrid();
	

	virtual int GetSceneType() = 0;
};
typedef CScene * LPSCENE;


