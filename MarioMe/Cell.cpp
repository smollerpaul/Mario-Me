#include "Cell.h"

void Cell::AddObject(CGameObject* obj)
{
	this->cellObjects.insert(obj);
}
