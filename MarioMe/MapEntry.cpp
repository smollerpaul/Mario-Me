#include "MapEntry.h"
#include "Game.h"
#include "Camera.h"

MapEntry::MapEntry(float xPos, float yPos, int nodeID, string sceneID, 
    string checkedSprite, string uncheckedSprite, 
    string adjList, string adjWeight, int worldNumber, bool isStartPos) {

    width = height = 48;
    nodeId = nodeID;
    sceneId = sceneID;
    this->x = xPos;
    this->y = yPos;
	this->isStartPos = isStartPos;

	if (worldNumber != NULL)
		this->worldNumber = worldNumber;

    if (!uncheckedSprite.empty() && !checkedSprite.empty()) {
        this->sprites["Unchecked"] = CSprites::GetInstance()->Get(uncheckedSprite);
        this->sprites["Checked"] = CSprites::GetInstance()->Get(checkedSprite);
    }

	vector<string> adjacents = split(adjList, ",");

	for (int i = 0; i < adjacents.size(); i++)
	{
		int adjNodeId = stoi(adjacents[i]);

		string moveDirection = "";
		switch (adjWeight[2 * i])
		{
		case 'l':
			moveDirection = "l";
			break;
		case 'u':
			moveDirection = "u";
			break;
		case 'r':
			moveDirection = "r";
			break;
		case 'd':
			moveDirection = "d";
			break;
		}

		if (moveDirection != "") {
			adjacentNodes[moveDirection] = adjNodeId;
		}
	}
}


void MapEntry::InitAnimations()
{
}

void MapEntry::Update(DWORD dt)
{
}

void MapEntry::Render()
{
    if (sprites.size() == 2) {
        Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
        int flip = 1;

        if (finished==0) {
            this->sprites["Unchecked"]->Draw(x - camera->GetX() + width/ 2, y - camera->GetY() + height / 2, flip);
        }
        else {
            this->sprites["Checked"]->Draw(x - camera->GetX() + width / 2, y - camera->GetY() + height / 2, flip);
        }
    }
}

bool MapEntry::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
    return true;
}

void MapEntry::Reset()
{
	finished = 0;
}

unordered_map<string, int> MapEntry::GetAdjacentNodes()
{
	return adjacentNodes;
}

void MapEntry::SetSceneFinished()
{
	finished = 1;
}

int MapEntry::GetNodeId()
{
	return nodeId;
}

void MapEntry::Enter()
{
	if (sceneId.empty()) return;
	if (finished==1) return;

	CGame::GetInstance()->SwitchScene(sceneId);
}

int MapEntry::GetObjectType()
{
    return ObjectType;
}
