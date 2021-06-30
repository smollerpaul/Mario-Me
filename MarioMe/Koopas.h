#pragma once
#include "GameObject.h"
#include "NormalKoopas.h"

class Camera;

class CKoopas : public CGameObject
{
public:
	NormalKoopas* objState = nullptr;

	CKoopas();
	CKoopas(NormalKoopas* objectState);
	void SetObjectState(NormalKoopas* objectState);
	virtual void InitAnimations() override;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void Update(DWORD dt) override;
	virtual void Render() override;
	virtual bool CanGetThrough(CGameObject* obj, float coEventNx, float coEventNy) override;

	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void BehaviorUpdate(DWORD dt) override;

	virtual int GetObjectType() override;
	static const int ObjectType = 2;

};