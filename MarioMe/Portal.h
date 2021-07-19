#pragma once

#include "GameObject.h"

class CPortal : public CGameObject
{
	string scene_id;	// target scene to switch to 

public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	string GetSceneId() { return scene_id;  }

	virtual int GetObjectType() override;
	static const int ObjectType = 13;
};

