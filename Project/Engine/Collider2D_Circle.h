#pragma once
#include "Collider2D.h"


class Collider2D_Circle :
    public Collider2D
{
public:
    Collider2D_Circle();
    Collider2D_Circle(const Collider2D_Circle& _other);
    virtual ~Collider2D_Circle();
    CLONE(Collider2D_Circle);

private:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;
    
public:
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

private:
    
public:
    void SetRadius(float _fRadius) { SetCollSize(Vec3(_fRadius, 1.f, 1.f)); }
    float GetRadius() const { return GetCollSize().x; }
};

