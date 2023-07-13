#pragma once
#include "cCollider2D.h"


class cCollider2D_Circle :
    public cCollider2D
{
public:
    cCollider2D_Circle();
    cCollider2D_Circle(const cCollider2D_Circle& _other);
    virtual ~cCollider2D_Circle();
    CLONE(cCollider2D_Circle);

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

