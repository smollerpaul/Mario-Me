#pragma once
#include "GameObject.h"
#include "NormalKoopas.h"

class Camera;
class CKoopas : public CGameObject
{
public:
	NormalKoopas* objState = NULL;

	CKoopas();
	CKoopas(NormalKoopas* objectState);
	void SetObjectState(NormalKoopas* objectState);
	virtual void InitAnimations() override;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void Update(DWORD dt) override;
	virtual void Render();
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void BehaviorUpdate(DWORD dt);

	virtual int GetObjectType() override;
	static const int ObjectType = 2;
};