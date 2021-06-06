#include "Scene.h"
#include "GameObject.h"



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

void CScene::RemoveObject(CGameObject* obj)
{
	objects.erase(remove_if(objects.begin(), objects.end(), []( CGameObject*& obj) {
		if (obj->GetAlive()!=0) {
			return false;
		}
		else {
			delete obj;
			return true;
		}
		}), objects.end());
}
