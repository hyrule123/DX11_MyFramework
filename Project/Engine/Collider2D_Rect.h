#pragma once
#include "Collider2D.h"
class Collider2D_Rect :
    public Collider2D
{
public:
    Collider2D_Rect();
    Collider2D_Rect(eCOLLIDER_TYPE_2D _Type);
    virtual ~Collider2D_Rect();
    CLONE(Collider2D_Rect)

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    //가로세로 길이와 Collider 클래스의 v3Offset을 활용하여 계산한다.
    enum XY { x, y };

public:




};

