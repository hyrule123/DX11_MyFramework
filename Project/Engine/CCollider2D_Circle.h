#pragma once
#include "CCollider2D.h"


class CCollider2D_Circle :
    public CCollider2D
{
public:
    CCollider2D_Circle();
    CCollider2D_Circle(const CCollider2D_Circle& _other);
    virtual ~CCollider2D_Circle();
    CLONE(CCollider2D_Circle);

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

