#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    //앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
    //eDIR_TYPE 열거체를 사용.
    Vec3  m_vRelativeDir[eDIR_TYPE::eDIR_END];

    Matrix  m_matWorld;

public:
    void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(const Vec3& _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z);  }

    void SetRelativePosX(float _x) { m_vRelativePos.x = _x; }
    void SetRelativePosY(float _y) { m_vRelativePos.y = _y; }
    void SetRelativePosZ(float _z) { m_vRelativePos.z = _z; }

    //Inline methods don't need const reference
    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRot() { return m_vRelativeRot; }
    Vec3 GetRelativeDir(eDIR_TYPE _Dir) { return m_vRelativeDir[(int)_Dir]; }





public:
    virtual void finaltick() override;    
    void UpdateData();


    CLONE(CTransform);    
public:
    CTransform();
    ~CTransform();
};

