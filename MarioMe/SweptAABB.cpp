#include "GameObject.h"
#include "Game.h"
#include "Utils.h"

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// obj B
	float ml, mt, mr, mb;		// obj chủ
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb); //get bbox of object B

	
	float svx, svy;
	coO->GetSpeed(svx, svy); //GET SPEED OF obj B

	float sdx = svx * dt; 
	float sdy = svy * dt;

	// close the gap between 2 moving objs ( if exists)
	float rdx = this->dx - sdx;  
	float rdy = this->dy - sdy;

	this->GetBoundingBox(ml, mt, mr, mb); // GET object chủ  stats (đang đọc trong hàm thằng nào thì nó là obj chủ)

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	// trả ra t, nx,ny ( time, phương react khi va chạm)

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);  //involve stats  và object B ( not chủ)
	// có ny, nx -> nice
	return e;
}

void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;

	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();
	// đang xét những va chạm có thể xảy ra gần nhất ( vì 1 lần va chạm đc 1 cái th chứ k nhiều cái duh)
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		// c->obj là object B đang xét, truyền vô là this (obj chủ)
		///goi 1 lna 2 ham, goi ham cua gooomba -> neu true, con obj chu dc di xuyen wa con B ( con obj chu la con dang chuyen dong)

		// goi ham can get through -> xem coi con nay co cho obj chu di ngang wa ko
		// //nếu gặp cái con đc truyền vô, có cho nó đi xuyen wa minh ko
		// goi ham cua con B, neu con B gap obj chu ma cho no di xuyen wa -> 
		if (c->obj->CanGetThrough(this, c->nx, c->ny) == true) {
			continue;
		}
		
		// truyyền stats của nx, ny của coEvents[i] vào hàm isGetThrough -> r xét cái số đó
		//if is getthrough true -> continue; if item can get through
		// nx, ny 1 trong 2 !=0 vì xem để biết va chạm phương x or y thôi

		if (c->t < min_tx && c->nx != 0) { 
			min_tx = c->t;
			nx = c->nx; 
			rdx = c->dx;
			min_ix = i; //smallest on x
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; 
			ny = c->ny; 
			rdy = c->dy;
			min_iy = i; //smallest on y
		}
	}

	//result is only 1 only, either x or y // cần get cái phương va chạm để define cái nào get through được
	if (min_ix >= 0) 
		coEventsResult.push_back(coEvents[min_ix]);

	if (min_iy >= 0) 
		coEventsResult.push_back(coEvents[min_iy]);
}


