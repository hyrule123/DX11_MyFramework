#pragma once
#include "ICollider2D.h"


class cCom_Coll2D_Circle :
    public ICollider2D
{
public:
    cCom_Coll2D_Circle();
    cCom_Coll2D_Circle(const cCom_Coll2D_Circle& _other);
    virtual ~cCom_Coll2D_Circle();
    CLONE(cCom_Coll2D_Circle);

private:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;
    
public:
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    
public:
    void SetRadius(float _fRadius) { SetCollSize(Vec3(_fRadius, 1.f, 1.f)); }
    float GetRadius() const { return GetCollSize().x; }
};

