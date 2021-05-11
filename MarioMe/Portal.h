#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	string scene_id;	// target scene to switch to 

	int width;
	int height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	string GetSceneId() { return scene_id;  }

	virtual int GetObjectType() override;
	static const int ObjectType = 13;
};

