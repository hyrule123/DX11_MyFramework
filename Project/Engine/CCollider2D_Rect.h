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
    //중점으로부터의 부호 구분이 있는 거리
    //ex)L, B는 항상 0 이하
    Vec4 m_v4LBRTLength;
    enum LBRT { L, B, R, T };

public:
    //중심점으로부터 각 변까지의 '거리'임에 주의할것.
    void SetLBRT(const Vec4& _v4DistFromCenter) { m_v4LBRTLength = _v4DistFromCenter; }
     

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;


};

