#include "Scene.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
	
}

Camera* CScene::GetCamera()
{
	return this->camera;
}

void CScene::SetCamera(Camera* camera)
{
	this->camera = camera;
}
