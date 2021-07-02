#pragma once
#include "Scene.h"
#include "MapMario.h"


class WorldScene :
    public CScene
{
protected:
	float camStartX = 0;
	float camStartY = 0;

	float camWidth = 0;
	float camHeight = 0;
public:
	MapMario* player=nullptr;

	WorldScene(string id, string filePath);
	virtual void Load() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void Unload() override;
	virtual void LoadMapObjects(string objectType, float x, float y, float width, float height) override;
	virtual void LoadMapEntries(float xPos, float yPos, int nodeID, string sceneID,
		string checkedSprite, string uncheckedSprite,
		string adjList, string adjWeight, int worldNumber, bool isStartPos) override;
	virtual void LoadBackground(float xPos, float yPos) override;
	MapMario* GetPlayer();

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	static const int SceneType = 777;
	virtual int GetSceneType();

};

