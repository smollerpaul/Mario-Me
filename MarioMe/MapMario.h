#pragma once
#include "GameObject.h"
#include "Entries.h"

class MapMario :
    public CGameObject
{
public:
	Entries* graph;

	int ny = 0;
	int currentNodeId=0;

	int targetNodeId=1;
	float targetX = 0;
	float targetY = 0;

	float dxGoal = 0;
	float dyGoal = 0;

	int canMove = 0;
	int currentMarioType = 1234;

	MapMario();
	virtual void InitAnimations();
	virtual void Update(DWORD dt);
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy);

	void AddNode(MapEntry* node);
	virtual void Render();
	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);
	bool CollisionHappen(float al, float at, float ar, float ab,
		float bl, float bt, float br, float bb);
	virtual int GetObjectType();
	static const int ObjectType = 12345;
};

