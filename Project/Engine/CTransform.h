#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
private:
    Vec3    m_vSize;

    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    //앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
    //eDIR_TYPE 열거체를 사용.
    Vec3    m_vRelativeDir[eDIR_TYPE::eDIR_END];

    //월드 방향(모든 회전정보 누적)
    Vec3    m_vWorldDir[eDIR_TYPE::eDIR_END];

    bool    m_bInheritScale;
    bool    m_bInheritRot;
    Matrix  m_matWorld;

public:
    //inline Setter
    void SetSize(const Vec3& _vSize) { m_vSize = _vSize; }

    void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(const Vec3& _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z);  }

    void SetRelativePosX(float _x) { m_vRelativePos.x = _x; }
    void SetRelativePosY(float _y) { m_vRelativePos.y = _y; }
    void SetRelativePosZ(float _z) { m_vRelativePos.z = _z; }

    void SetScaleInheritance(bool _bInherit) { m_bInheritScale = _bInherit; }
    void SetRotInheritance(bool _bInherit) { m_bInheritRot = _bInherit; }

    //inline Getter
    ////Inline methods don't need to return the value by const reference
    Vec3 GetSize() const { return m_vSize; }
    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetWorldPos() const { return Vec3(m_matWorld.m[3]); }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetWorldScale() const;
    Vec3 GetRelativeRot() const { return m_vRelativeRot; }

    Matrix GetWorldRotMat() const;
    Vec3 GetWorldRot(eAXIS3D_TYPE _eAxis) const;

    Vec3 GetRelativeDir(eDIR_TYPE _eDir) const { return m_vRelativeDir[(int)_eDir]; }
    Vec3 GetWorldDir(eDIR_TYPE _eDir) const { return Vec3(m_matWorld.m[(int)_eDir]).Normalize(); }
    Matrix GetWorldMat() const { return m_matWorld; }



public:
    virtual void finaltick() override;    
    void UpdateData();


    CLONE(CTransform);    
public:
    CTransform();
    //단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
    ~CTransform();
};

inline Vec3 CTransform::GetWorldScale() const
{
    return Vec3(m_matWorld.Right().Length(), m_matWorld.Up().Length(), m_matWorld.Front().Length());
}

inline Matrix CTransform::GetWorldRotMat() const
{
    return Matrix(m_matWorld.Right().Normalize(), m_matWorld.Up().Normalize(), m_matWorld.Front().Normalize());
}

inline Vec3 CTransform::GetWorldRot(eAXIS3D_TYPE _eAxis) const
{
    return m_matWorld.Axis(_eAxis).Normalize();
}
