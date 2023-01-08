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
    Vec3  m_vRelativeDir[eDIR_TYPE::eDIR_END];

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
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRot() const { return m_vRelativeRot; }
    Vec3 GetRelativeDir(eDIR_TYPE _Dir) const { return m_vRelativeDir[(int)_Dir]; }
    Matrix GetWorldMat() const { return m_matWorld; }



public:
    virtual void finaltick() override;    
    void UpdateData();


    CLONE(CTransform);    
public:
    CTransform();
    ~CTransform();
};

