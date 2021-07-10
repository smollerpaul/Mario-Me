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
	float t, nx, ny, tl;

	coO->GetBoundingBox(sl, st, sr, sb); //get bbox of object B

	float svx, svy;
	coO->GetSpeed(svx, svy); //GET SPEED OF obj B

	float sdx = svx * dt; 
	float sdy = svy * dt;

	// close the gap between 2 moving objs ( if exists)
	// trừ cái dx, dy là khoảng cách đã update rồi của 
	float rdx = this->dx - sdx;  
	float rdy = this->dy - sdy;

	this->GetBoundingBox(ml, mt, mr, mb); // GET object chủ  stats (đang đọc trong hàm thằng nào thì nó là obj chủ)

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny, tl
	);

	// trả ra t, nx,ny ( time, phương react khi va chạm)

	CCollisionEvent* e = new CCollisionEvent(t, tl, nx, ny, rdx, rdy, coO);  //involve stats  và object B ( not chủ)
	// có ny, nx -> nice
	return e;
}



void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	vector<LPCOLLISIONEVENT> temp;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t >= 0 && e->t <= 1.0f && e->tl > 0)
			temp.push_back(e);
		else delete e;
	}

	std::sort(temp.begin(), temp.end(), CCollisionEvent::compare);

	for each (LPCOLLISIONEVENT coll in temp)
	{
		for each (LPCOLLISIONEVENT result in coEvents) {	
			if (result->obj->CanGetThrough(this, result->nx, result->ny) == true) {
				//DebugOut(L" calc Potential Bỏ qua nè: ny: %f\n", result->ny);
				continue;
			}

			float sl, st, sr, sb;		
			float ml, mt, mr, mb;		
			float t, nx, ny, tl;

			coll->obj->GetBoundingBox(sl, st, sr, sb); 

			float svx, svy;
			coll->obj->GetSpeed(svx, svy); 

			float sdx = svx * dt; 
			float sdy = svy * dt;

			float rdx = this->dx - sdx;  
			float rdy = this->dy - sdy;

			if (coll->nx != 0) {
				rdy = rdy * result->t;
			}
			else {
				rdx = rdx * result->t;
			}

			this->GetBoundingBox(ml, mt, mr, mb); 

			CGame::SweptAABB(
				ml, mt, mr, mb,
				rdx, rdy,
				sl, st, sr, sb,
				t, nx, ny, tl
			);

			// trả ra t, nx,ny ( time, phương react khi va chạm)
			CCollisionEvent* e = new CCollisionEvent(t, tl, nx, ny, rdx, rdy, coll->obj);  //involve stats  và object B

			//no collision
			if (e->t < 0 || e->t > 1.0f || e->tl == 0) {
				coll->t = -100;
			}
			delete e;
		}

		//push into temp after sweptAABBEx
		if (coll->t >= 0 && coll->t <= 1.0f && coll->tl > 0) {

			if (coll->obj->CanGetThrough(this, coll->nx, coll->ny) == true) {
				//DebugOut(L" calc Potential Bỏ qua nè: ny: %f\n", result->ny);
				continue;
			}

			coEvents.push_back(coll);

			if (this->GetObjectType() == 987 && coll->obj->GetObjectType() == 12) {
				//DebugOut(L"MARIO cham GHOST %f  %f\n", coll->t, coll->tl);

				//DebugOut(L"CALC đáng nhẽ k hiện ra if NY=1 !\n");
				//float sl, st, sr, sb;		// obj B
				//float ml, mt, mr, mb;
				//coll->obj->GetBoundingBox(sl, st, sr, sb); 
				//this->GetBoundingBox(ml, mt, mr, mb);

				//DebugOut(L" nx: %f, ny: %f \n", coll->nx, coll->ny);
				/*DebugOut(L"POS GHOST: %f %f %f %f\n", sl, st, sr, sb);
				DebugOut(L"POS MARIO: %f %f %f %f\n", ml, mt, mr, mb);*/
			}
		}
		else
			delete coll;
	}

	//DebugOut(L"coEventsResult = %d\n", coEventsResult.size());
}



void CGameObject::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;

	/*int min_ix = -1;
	int min_iy = -1;*/

	nx = 0.0f;
	ny = 0.0f;

	//DebugOut(L"coEvents = %d\n", coEvents.size());
	//DebugOut(L"coEventsResult = %d\n", coEventsResult.size());

	coEventsResult.clear();
	
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		coEventsResult.push_back(c);
		if (c->obj->CanGetThrough(this, c->nx, c->ny) == true) {
		//	DebugOut(L"filter collision BỎ QUA NÈ:  ny:  %f \n", c->ny);
			continue;
		}

		if (this->GetObjectType() == 987 && c->obj->GetObjectType() == 12) {
		//	DebugOut(L"filter collision:  ny:  %f \n", c->ny);
		}
	
		if (c->t < min_tx && c->nx != 0) { 
			min_tx = c->t;
			nx = c->nx; 
			rdx = c->dx;
			//min_ix = i; //smallest on x
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; 
			ny = c->ny; 
			rdy = c->dy;
			//min_iy = i; //smallest on y
		}
	}


	//result is only 1 only, either x or y // cần get cái phương va chạm để define cái nào get through được
	/*if (min_ix >= 0) 
		coEventsResult.push_back(coEvents[min_ix]);

	if (min_iy >= 0) 
		coEventsResult.push_back(coEvents[min_iy]);*/
}


