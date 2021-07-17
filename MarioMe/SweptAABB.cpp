#include "GameObject.h"
#include "Game.h"
#include "Utils.h"

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{

	float sl, st, sr, sb;		
	float ml, mt, mr, mb;		
	float t, nx, ny, tl;

	coO->GetBoundingBox(sl, st, sr, sb); 

	float svx, svy;
	coO->GetSpeed(svx, svy); 

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
		//if (coObjects->at(i)!= nullptr) {
			//DebugOut(L"ello pre-calc: %d \n", coObjects->at(i)->GetObjectType());
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t >= 0 && e->t <= 1.0f && e->tl > 0)
				temp.push_back(e);
			else delete e;
		//}
	}

	std::sort(temp.begin(), temp.end(), CCollisionEvent::compare);

	for each (LPCOLLISIONEVENT coll in temp)
	{
		for each (LPCOLLISIONEVENT result in coEvents) {	
			if (result->obj->CanGetThrough(this, result->nx, result->ny) == true) {
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

			//calc relative dx from previous calc
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

			CCollisionEvent* e = new CCollisionEvent(t, tl, nx, ny, rdx, rdy, coll->obj);  

			//no collision
			if (e->t < 0 || e->t > 1.0f || e->tl == 0) {
				coll->t = -100;
			}
			delete e;
		}

		if (coll->t >= 0 && coll->t <= 1.0f && coll->tl > 0)
		{
			/*if (coll->obj->CanGetThrough(this, coll->nx, coll->ny) == true) {
				continue;
			}*/

			coEvents.push_back(coll);
			
			/*if (this->GetObjectType() == 22 && coll->obj->GetObjectType() == 987) {
				DebugOut(L"1 BLOCK-> MARIO   %f  %f %f \n", coll->nx, coll->ny, coll->tl);

				float sl, st, sr, sb;		
				float ml, mt, mr, mb;
				coll->obj->GetBoundingBox(sl, st, sr, sb); 
				this->GetBoundingBox(ml, mt, mr, mb);

				DebugOut(L"POS MARIO: %f %f %f %f\n", sl, st, sr, sb);
				DebugOut(L"POS QB: %f %f %f %f\n", ml, mt, mr, mb);

				DebugOut(L"----------------------\n");

			}
				*/

			/*if (this->GetObjectType() == 987 && coll->obj->GetObjectType() == 22) {
				DebugOut(L"1 MARIO->BLOCK   %f  %f  %f \n", coll->nx, coll->ny, coll->tl);

				float sl, st, sr, sb;
				float ml, mt, mr, mb;
				coll->obj->GetBoundingBox(sl, st, sr, sb);
				this->GetBoundingBox(ml, mt, mr, mb);

				DebugOut(L"POS QB: %f %f %f %f\n", sl, st, sr, sb);
				DebugOut(L"POS MARIO: %f %f %f %f\n", ml, mt, mr, mb);

				DebugOut(L"----------------------\n");
			}*/
	

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

	nx = 0.0f;
	ny = 0.0f;

	//DebugOut(L"coEvents = %d\n", coEvents.size());
	

	coEventsResult.clear();
	
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->obj->CanGetThrough(this, c->nx, c->ny) == true) {
			continue;
		}

		if (this->GetObjectType() == 5) {
			//DebugOut(L"this: goomba C obj: %d \n", c->obj->GetObjectType());
		}

		if (this->GetObjectType() ==22) {
			//if(c->obj->GetObjectType()== 4002)
				//DebugOut(L" TAIL TOUCHES : %d  nx: %f  ny: %f \n", c->obj->GetObjectType(),c->nx, c->ny );

		}

		if (this->GetObjectType() == 4002) {
			//DebugOut(L"C obj: %d \n", c->obj->GetObjectType());
		}
	
		if (this->GetObjectType() == 987 && c->obj->GetObjectType() == 22) {
			///DebugOut(L"2 MARIO BLOCK_ nX:  %f    NY: %F \n", c->nx, c->ny);
		}

		if (this->GetObjectType() == 22 && c->obj->GetObjectType() == 987) {
			//DebugOut(L"2 BLOCK MARIO_ nX:  %f    NY: %F \n", c->nx, c->ny);
		}

		if (this->GetObjectType() == 2002 && c->obj->GetObjectType() == 22) {

			//DebugOut(L"  TAIL BLOCK_ nX:  %f    NY: %F  TL: %f \n",c->nx, c->ny, c->tl);
		}

		if (this->GetObjectType() == 22 && c->obj->GetObjectType() == 2002) {

			//DebugOut(L"  BLOCK TAIL  nX:  %f    NY: %F  TL: %f \n", c->nx, c->ny, c->tl);
		}

		if (this->GetObjectType() == 4002 && c->obj->GetObjectType() == 987) {
			//DebugOut(L"filter collision:   LEAF MARIO_ nX:  %f    NY: %F \n", c->nx, c->ny);
		}

		if (this->GetObjectType() == 987 && c->obj->GetObjectType() == 4002) {
			//DebugOut(L"filter collision:   MARIO LEAF_ nX:  %f    NY: %F \n", c->nx, c->ny);
		}
	
		if (c->t < min_tx && c->nx != 0) { 
			min_tx = c->t;
			nx = c->nx; 
			rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; 
			ny = c->ny; 
			rdy = c->dy;
		}

		if(c->obj->GetObjectType())
			coEventsResult.push_back(c);

	}
	/*if (min_ix >= 0) 
		coEventsResult.push_back(coEvents[min_ix]);

	if (min_iy >= 0) 
		coEventsResult.push_back(coEvents[min_iy]);*/
}


