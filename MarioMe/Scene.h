#pragma once
#include "Camera.h"

#include <d3dx9.h>
#include <string>

using namespace std;

class CScene
{
protected:
	string id;
	string sceneFilePath;

	Camera* camera;

public: 
	CScene(string id, string filePath);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 

	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;

	Camera* GetCamera();
	void SetCamera(Camera* camera);

};
typedef CScene * LPSCENE;


