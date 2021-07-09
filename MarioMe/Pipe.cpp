#include "Pipe.h"
#include "Game.h"
#include "Camera.h"

Pipe::Pipe(int length, int type, float width, float height, int direction)
{
	renderOrder = 99;
	this->width = width;
	this->height = height;

	this->pipeLength = length;
	this->singlePieceSize = height / pipeLength; 
	pipeDir = direction;

	this->head = CSprites::GetInstance()->Get("spr-pipe-" + to_string(type) + "-vertical-head");
	this->body = CSprites::GetInstance()->Get("spr-pipe-" + to_string(type) + "-vertical-body");
}

void Pipe::Render()
{
	if (this->head == NULL || this->body == NULL) {
		return;
	}

	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	float l, t, b, r;
	GetBoundingBox(l, t, r, b);

	float renderX = x - camera->GetX();
	float renderY = y - camera->GetY();

	switch (pipeDir) {
	case 2:
		head->Draw(renderX + PIPE_BLOCK_WIDTH / 2, renderY + height - singlePieceSize + PIPE_BLOCK_HEIGHT/ 2, 1);
		for (int i = renderY; i < renderY + height - singlePieceSize - 1; i += singlePieceSize) {
			body->Draw(renderX+ PIPE_BLOCK_WIDTH/ 2, i + PIPE_BLOCK_HEIGHT / 2, 1);
		}
		break;
	case 1:
		head->Draw(renderX + PIPE_BLOCK_WIDTH /2, renderY + PIPE_BLOCK_HEIGHT / 2, 1);
		for (int i = renderY + singlePieceSize; i < renderY + height - 1; i += singlePieceSize) {
			body->Draw(renderX + PIPE_BLOCK_WIDTH/ 2, i + PIPE_BLOCK_HEIGHT / 2, 1);
		}
		break;

	}
//	RenderBoundingBox();

}

bool Pipe::CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy)
{
	return false;
}

int Pipe::GetObjectType()
{
	return ObjectType;
}
