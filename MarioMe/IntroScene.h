#pragma once
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Utils.h"

class CMario;
class Text;
class ArrowPicker;

class IntroScene :
    public CScene
{
protected:
	CMario* m1 = nullptr;
	CMario* m2 = nullptr;

	Text* player1 = nullptr;
	Text* player2 = nullptr;

	float camStartX = 0;
	float camStartY = 0;
	float camWidth = 0;
	float camHeight = 0;

	int scriptStep = -1;
	float stepTimer = 0;

	float m1XBegin = 10;
	float m2XBegin = 688;

	ArrowPicker* arrow = nullptr;

public:
	IntroScene(string id, string filePath);

	virtual void Load() override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual void Unload() override;
	virtual void LoadMapObjects(string objectName, vector< D3DXVECTOR2> cells, float x, float y, float width = 0, float height = 0) override;
	void ScriptUpdate(DWORD dt);

	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void ResetStepTimer();
	static const int SceneType = 778;
	virtual int GetSceneType();
};

