#pragma once
#include "ICollider2D.h"
class cCom_Coll2D_Rect :
    public ICollider2D
{
public:
    cCom_Coll2D_Rect();
    cCom_Coll2D_Rect(eCOLLIDER_TYPE_2D _Type);
    virtual ~cCom_Coll2D_Rect();
    CLONE(cCom_Coll2D_Rect)

public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

public:
    virtual void UpdatecCollider() override;
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    //가로세로 길이와 ICollider 클래스의 v3Offset을 활용하여 계산한다.
    enum XY { x, y };

public:




};

