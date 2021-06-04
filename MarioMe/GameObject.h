#pragma once
#include "Utils.h"

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class Camera;
class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x; 
	float y;

	float width;
	float height;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	float gravity;

	int nx;	 
	int flip;

	int state;
	int alive;

	DWORD dt;

	unordered_map<string, LPANIMATION> animations;

public: 
	CGameObject();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void UpdatePosition();
	virtual void Render() = 0;

	void SetPosition(float x, float y);
	void GetPosition(float& x, float& y);

	void GetPosX(float& x);
	void GetPosY(float& y);

	void SetSpeed(float vx, float vy);
	void GetSpeed(float& vx, float& vy);

	void SetSize(float w, float h);
	void GetSize(float& w, float& h);

	void SetDirection(int nx);
	int GetDirection();
	
	virtual void SetState(int state);
	int GetState();

	void SetAlive(int alive);
	int GetAlive();

	virtual int GetObjectType() ;

	virtual void InitAnimations();

	void SetFlipOnNormal(int nx);
	
	virtual void GetBoundingBox(
		float& left, float& top, 
		float& right, float& bottom);
	void RenderBoundingBox();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	~CGameObject();
};

