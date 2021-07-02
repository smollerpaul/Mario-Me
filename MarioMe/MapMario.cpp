#include "MapMario.h"
#include "Utils.h"
#include "Game.h"
#include"PlayerData.h"
#include"Mario.h"

MapMario::MapMario()
{
	width = height = 48;
	targetNodeId = 1;
	currentNodeId = 0;
	this->graph = new Entries();
	vx = vy=  0.3;
}

void MapMario::InitAnimations()
{
	if (animations.size() < 1 ) {
		currentMarioType = PlayerData::GetInstance()->GetMarioType();

		if (currentMarioType == SmallMario::ObjectType) {
			animations["Default"] = CAnimations::GetInstance()->Get("ani-small-mario-map")->Clone();
		}
		if (currentMarioType == BigMario::ObjectType) {
			animations["Default"] = CAnimations::GetInstance()->Get("ani-big-mario-map")->Clone();
		}
		if (currentMarioType == RacoonMario::ObjectType) {
			animations["Default"] = CAnimations::GetInstance()->Get("ani-raccoon-mario-map")->Clone();
		}
		if (currentMarioType == FireMario::ObjectType) {
			animations["Default"] = CAnimations::GetInstance()->Get("ani-fire-mario-map")->Clone();
		}
	}
}

void MapMario::Update(DWORD dt)
{
	if (canMove == 1) {
		dx = targetX - x;
		dy = targetY - y;

		//define movement direction
		if (dx > 0)
			x += vx*dt;
		else if( dx < 0)
			x -= vx * dt;

		if (dy > 0)
			y += vy * dt;
		else if (dy < 0)
			y -= vy * dt;

		//k hiu so nay lam
		float al = x + 21;
		float at = y + 21;
		float ar = x + 27;
		float ab = y + 27;

		float bl = x + 23;
		float bt = y + 23;
		float br = x + 25;
		float bb = y + 25;

		if (CollisionHappen(al, at, ar, ab, bl, bt, br, bb) == true) {
			canMove = 0;
			x = targetX;
			y = targetY;
			currentNodeId = targetNodeId;
		}
	}
}

bool MapMario::CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy)
{
	return false;
}

void MapMario::CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT> coEvents)
{
}

void MapMario::BehaviorUpdate(DWORD dt, vector<LPCOLLISIONEVENT> coEventsResult, vector<LPCOLLISIONEVENT> coEvents)
{
}

void MapMario::AddNode(MapEntry* node)
{
	graph->AddNode(node);


	if (node->isStartPos==true) {
		currentNodeId= node->GetNodeId();
		x = node->x;
		y = node->y;
	}
}

void MapMario::Render()
{
	InitAnimations();
	CAnimation* ani = this->animations["Default"];

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	ani->Render(x - camera->GetX() + width/ 2, y - camera->GetY() + height/ 2, flip);
}

void MapMario::OnKeyUp(int keyCode)
{
}

void MapMario::OnKeyDown(int keyCode)
{
	string direction = "";
	switch (keyCode)
	{
	case DIK_LEFT:
		direction = "l";
		break;
	case DIK_RIGHT:
		direction = "r";
		break;
	case DIK_UP:
		direction = "u";
		break;
	case DIK_DOWN:
		direction = "d";
		break;

	case DIK_S:
		graph->GetNode(currentNodeId)->Enter();
		break;

	default:
		break;
	}

	MapEntry* current = graph->GetNode(currentNodeId);
	unordered_map<string, int> adjList = current->GetAdjacentNodes();
	
	if (adjList.find(direction) == adjList.end())
		return;
	else {
		canMove = 1;
		MapEntry* nextTargetNode = graph->GetNode(adjList.at(direction));
		targetX = nextTargetNode->x;
		targetY = nextTargetNode->y;

		targetNodeId = nextTargetNode->GetNodeId();
	}
}

bool MapMario::CollisionHappen(float al, float at, float ar, float ab, 
	float bl, float bt, float br, float bb)
{
	return !(ar <= bl || al >= br || at >= bb || ab <= bt);
}

int MapMario::GetObjectType()
{
	return ObjectType;
}
