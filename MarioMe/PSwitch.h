#pragma once
#include "GameObject.h"
#define PSWITCH_SIZE		45

class PSwitch :
    public CGameObject
{
public:
	int activated = 0;
	int visible = 0;
	float beginY = 0;
	int scriptStep = 0;

	PSwitch();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual void SetPosition(float x, float y) override;
	virtual void Render();
	virtual void InitAnimations() override;
	virtual void Update(DWORD dt) override;
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void BehaviorUpdate(DWORD dt);
	void SetVisible(int vs);

	virtual int GetObjectType() override;
	static const int ObjectType = 16;
};

