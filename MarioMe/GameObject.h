#pragma once
#include "Utils.h"
#include "Sprites.h"
#include "Animations.h"
#include "EffectVault.h"

using namespace std;

class Camera;
class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float tl;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float tl, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
		this->tl = tl;
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
	//day a ha?
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	CGameObject();
	virtual void Update(DWORD dt);
	virtual void UpdatePosition();
	
	virtual void Render() = 0;

	virtual bool CanGetThrough(CGameObject* gameObjToCollide, float coEventNx, float coEventNy);
	virtual void SetPosition(float x, float y);
	
	void GetPosition(float& x, float& y);

	float GetVx();
	float GetVy();

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

	int GetNormalX();

	void SetAlive(int alive);
	int GetAlive();

	virtual int GetObjectType() ;

	virtual void InitAnimations();

	void SetFlipOnNormal(int nx);
	void SetFlipOnNormalEnemy(int nx);
	
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
	void ClearCollision();
	virtual void CollisionUpdate(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void BehaviorUpdate(DWORD dt);

	~CGameObject();
};

