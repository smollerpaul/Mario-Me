#include "Scene.h"



CScene::CScene(string id, string filePath)
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

GameMap* CScene::GetMap()
{
	return this->map;
}

void CScene::AddObject(CGameObject* obj)
{
	objects.push_back(obj);
}
