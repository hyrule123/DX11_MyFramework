#include "pch.h"
#include "CSpatialGrid.h"

#include "CCollider.h"

CSpatialGrid::CSpatialGrid()
{
}

CSpatialGrid::~CSpatialGrid()
{
}

void CSpatialGrid::SortVecCollider()
{
	std::sort(m_vecpColliderInside.begin(), m_vecpColliderInside.end(), 
		[](CCollider* _a, CCollider* _b)
		{
			return (_a->GetID() < _b->GetID());
		}
	);
}