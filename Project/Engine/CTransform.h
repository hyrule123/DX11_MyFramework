#pragma once
#include "CComponent.h"


class CTransform :
    public CComponent
{
private:
    Matrix    m_matSize;

    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    //앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
    //eDIR_TYPE 열거체를 사용.
    Vec3    m_vRelativeDir[eDIR_TYPE::eDIR_END];

    //월드 방향(모든 회전정보 누적)
    Vec3    m_vWorldDir[eDIR_TYPE::eDIR_END];

    //부모로부터 상속받지 않은 상대적 행렬
    Matrix  m_matRelative;

    //부모로부터 누적된 트랜스폼 정보
    Matrix  m_matParent;

    bool    m_bInheritScale;
    bool    m_bInheritRot;
    
    //부모로부터 상속받아 최종적으로 만들어진 월드행렬
    Matrix  m_matWorld;

    //상속 형태로 업데이트가 필요한지 여부를 저장.
    //위치가 변하지 않았는데 굳이 월드행렬을 업데이트 할 필요가 없음.
    //m_matRelative를 업데이트 해야하는지 여부가 저장되어있음.
    //자신의 행렬을 업데이트 해야 한다면 반드시 부모 행렬을 받아와야 하기 때문에 m_bNeedParentUpdate도 업데이트 해준다.
    bool    m_bNeedMyUpdate;

    //자신이 아니라 부모님의 행렬이 업데이트되었을 경우 - 자신의 월드행렬은 업데이트할 필요가 없다.
    bool    m_bNeedParentUpdate;

public:
    //inline Setter

    //사이즈의 경우 전체를 갱신할 필요 없음.
    void SetSize(const Vec3& _vSize) { m_matSize._11 = _vSize.x; m_matSize._22 = _vSize.y; m_matSize._33 = _vSize.z; }

    void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; SetMyUpdate(); }
    void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; SetMyUpdate(); }
    void SetRelativeRot(const Vec3& _vRot) { m_vRelativeRot = _vRot; SetMyUpdate(); }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); SetMyUpdate(); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); SetMyUpdate(); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativePosX(float _x) { m_vRelativePos.x = _x; SetMyUpdate(); }
    void SetRelativePosY(float _y) { m_vRelativePos.y = _y; SetMyUpdate(); }
    void SetRelativePosZ(float _z) { m_vRelativePos.z = _z; SetMyUpdate(); }

    void SetScaleInheritance(bool _bInherit) { m_bInheritScale = _bInherit; SetMyUpdate(); }
    void SetRotInheritance(bool _bInherit) { m_bInheritRot = _bInherit; SetMyUpdate(); }

    //이번 틱에 업데이트를 해야한다고 설정. 자신의 움직임에 영향을 받는 자식 오브젝트들에게도 재귀적으로 알림
    void SetMyUpdate();
    void SetParentUpdate() { m_bNeedParentUpdate = true; }

    //inline Getter
    ////Inline methods don't need to return the value by const reference
    Vec3 GetSize() const { return Vec3(m_matSize._11, m_matSize._22, m_matSize._33); }
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

private:
    void UpdateMyTransform();

    //부모의 트랜스폼 행렬을 받아서 최종적인 월드행렬을 업데이트 한다.
    void UpdateParentMatrix();

public:
    virtual void finaltick() override;    
    virtual void cleanup() override {};
    void UpdateData();


    CLONE(CTransform);    
public:
    CTransform();
    //단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
    ~CTransform();
};


inline void CTransform::SetMyUpdate()
{
    //이미 설정이 되어 있다면 return
    if (true == m_bNeedMyUpdate)
        return;
    m_bNeedMyUpdate = true; 
    GetOwner()->SetChildTransformToUpdate();
}


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
