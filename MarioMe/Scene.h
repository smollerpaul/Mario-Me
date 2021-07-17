#pragma once
#include "Utils.h"

using namespace std;

class Camera;
class CGameObject;
class GameMap;

class CScene
{
protected:
	string id;
	string sceneFilePath;

	Camera* camera;
	vector<CGameObject*> objects;
	GameMap* map = NULL;

public: 

	int pSwitchActivated = 0;
	CScene();
	CScene(string id, string filePath);
	virtual void LoadMapObjects(string objectName, float x, float y, float width, float height);
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	virtual void LoadMapEntries(float xPos, float yPos, int nodeID, string sceneID,
		string checkedSprite, string uncheckedSprite,
		string adjList, string adjWeight,int worldNumber, bool isStartPos);

	virtual void LoadBackground(float xPos, float yPos);
	virtual void LoadSpecialObject(float xPos, float yPos, float width, float height, int length=0, int type=0, int direction=0);
	Camera* GetCamera();
	GameMap* GetMap();

	void SetCamera(Camera* camera);
	void AddObject(CGameObject* obj);
	void RemoveObject();


	virtual int GetSceneType()=0;
};
typedef CScene * LPSCENE;


