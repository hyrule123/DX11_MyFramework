#pragma once
#include "CCollider2D.h"
class CCollider2D_Rect :
    public CCollider2D
{
public:
    CCollider2D_Rect();
    CCollider2D_Rect(eCOLLIDER_TYPE_2D _Type);
    virtual ~CCollider2D_Rect();
    CLONE(CCollider2D_Rect)

private:
    
    //가로세로 길이와 CCollider 클래스의 v3Offset을 활용하여 계산한다.
    enum XY { x, y };
    Vec2 m_v2RectSize;

public:
    void SetSize(Vec2 _v2RectSize) { m_v2RectSize = _v2RectSize; }

    //https://m.ygosu.com/board/st/268087/?page=1 참고
    //사이즈와 오프셋값으로 변환(V4값이 LRBT임에 주의할 것)
    void SetSCBuildingSize(UINT _uNumMegatileX, UINT _uNumMegatileY, const Vec4& _v4LBRTOffset);
     

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;
};

