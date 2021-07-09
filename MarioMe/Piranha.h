#pragma once
#include "GameObject.h"

#define PIR_WIDTH		48
#define PIR_HEIGHT		66
class Piranha :
    public CGameObject
{
public:
	float revealTimer = 0;
	float waitTimer = 0;
	int scriptStep = 0;

	float beginY = 0;

	Piranha();
	virtual void SetPosition(float xPos, float yPos) override;

	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual void InitAnimations() override;

	virtual int GetObjectType() override;
	static const int ObjectType = 86;
};

