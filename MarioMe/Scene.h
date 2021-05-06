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
	CScene(string id, string filePath);

	virtual void LoadMapObjects(string objectName, float x, float y, float width, float height);
	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 

	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;

	Camera* GetCamera();
	GameMap* GetMap();

	void SetCamera(Camera* camera);
	void AddObject(CGameObject* obj);

};
typedef CScene * LPSCENE;


