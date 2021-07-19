#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"


CScene::CScene() {
	this->camera = new Camera();
}

CScene::CScene(string id, string filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

void CScene::LoadMapObjects(string objectName, float x, float y, float width, float height)
{
}

void CScene::LoadBackground(float xPos, float yPos)
{
}

Camera* CScene::GetCamera()
{
	return this->camera;
}


void CScene::LoadSpecialObject(string objectType, float xPos, float yPos, float width, float height,int length, int type, int direction, float desX, float desY , int moveDir, int targetReg)
{
}

void CScene::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void CScene::LoadMapEntries(float xPos, float yPos, int nodeID, string sceneID, string checkedSprite, string uncheckedSprite, 
	string adjList, string adjWeight,int worldNumber, bool isStartPos)
{
}

GameMap* CScene::GetMap()
{
	return this->map;
}

void CScene::AddObject(CGameObject* obj)
{
	objects.push_back(obj);
}

void CScene::RemoveObject()
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

void CScene::Render()
{
}

