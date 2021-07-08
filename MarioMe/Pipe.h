#pragma once
#include "GameObject.h"

#define PIPE_BLOCK_HEIGHT 48
#define PIPE_BLOCK_WIDTH  96
class Pipe :
    public CGameObject
{
public:
	LPSPRITE head;
	LPSPRITE body;

	int pipeLength = 0;
	float singlePieceSize = 0;
	int pipeDir = 0;

	Pipe(int length, int type, float width, float height, int direction);

	virtual void Render() override;
	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy) override;
	virtual int GetObjectType() override;
	static const int ObjectType = 17;
};

