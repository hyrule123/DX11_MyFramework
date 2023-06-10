#include "pch.h"
#include "SC_Func.h"

#include <Engine/CCollider.h>

void SC_Func::SetSCBuildingSize(CCollider* _pCol, int _uNumMegatileX, int _uNumMegatileY, const Vec4& _v4LRTBOffset)
{
    enum LBRT { L, R, T, B };
    //메가타일에서의 사이즈
    Vec2 v2SizeInPixel = Vec2(32.f * (float)_uNumMegatileX, 32.f * (float)_uNumMegatileY);

    Vec3 RectSize = Vec3(v2SizeInPixel.x - (_v4LRTBOffset[L] + _v4LRTBOffset[R]), v2SizeInPixel.y - (_v4LRTBOffset[B] + _v4LRTBOffset[T]), 1.f);

    _pCol->SetFollowTransformSize(false);
    _pCol->SetCollSize(RectSize);
    _pCol->SetOffsetPos(Vec3(_v4LRTBOffset[L] - _v4LRTBOffset[R], _v4LRTBOffset[B] - _v4LRTBOffset[T], 0.f));
}
