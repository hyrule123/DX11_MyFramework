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

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    //가로세로 길이와 CCollider 클래스의 v3Offset을 활용하여 계산한다.
    enum XY { x, y };

public:




};

