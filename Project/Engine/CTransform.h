#pragma once
#include "CComponent.h"


class CTransform :
    public CComponent
{
public:
    CTransform();
    //단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
    ~CTransform();
    CLONE(CTransform);

public:
    virtual void finaltick() override;
    virtual void cleanup() override {};
    //void UpdateData();

public:
    bool SaveJson(Json::Value* _pJson) override;
    bool LoadJson(Json::Value* _pJson) override;

public:


private:
    Vec3    m_v3Size;

    Vec3    m_v3RelativeScale;

    //Scale 정보가 1, 1, 1(기본값)이 아닐 경우 true
    //Scale 정보가 단위행렬인데 계산할 필요는 없으므로 
    bool    m_bIsDefaultScale;

    Vec3    m_v3RelativePos;
    Vec3    m_v3RelativeRot;

    float m_fLongestDiagonalLen;

    //앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
    //eDIR_TYPE 열거체를 사용.
    Vec3    m_vRelativeDir[(UINT)eDIR_TYPE::END];

    //월드 방향(모든 회전정보 누적)
    Vec3    m_vWorldDir[(UINT)eDIR_TYPE::END];

    //부모로부터 상속받지 않은 상대적 행렬
    Matrix  m_matRelative;

    //부모로부터 누적된 트랜스폼 정보
    Matrix  m_matParent;

    bool    m_bInheritScale;
    bool    m_bInheritRot;
    bool    m_bLockRotation; //자신의 회전 방지
    
    //자신의 사이즈 정보가 반영되지 않은 행렬
    //자식 Transform에 값을 넘겨줄 떄 이 행렬로 넘겨줘야 해서 별도로 분리함.
    Matrix  m_matWorldWithoutSize;

    //부모로부터 상속받아 최종적으로 만들어진 월드행렬
    Matrix  m_matWorld;
    

    //상속 형태로 업데이트가 필요한지 여부를 저장.
    //위치가 변하지 않았는데 굳이 월드행렬을 업데이트 할 필요가 없음.
    //m_matRelative를 업데이트 해야하는지 여부가 저장되어있음.
    //자신의 행렬을 업데이트 해야 한다면 반드시 부모 행렬을 받아와야 하기 때문에 m_bNeedParentUpdate도 업데이트 해준다.
    bool    m_bNeedMyUpdate;

    //자신이 아니라 부모님의 행렬이 업데이트되었을 경우 - 자신의 월드행렬은 업데이트할 필요가 없다.
    bool    m_bNeedParentUpdate;

    //Size는 자신에게만 적용되는 고유값이므로 재귀형으로 전달할 필요 없음.
    bool    m_bSizeUpdated;

public:
    //inline Setter

    //사이즈의 경우 전체를 갱신할 필요 없음.
    void SetSize(const Vec3& _vSize);

    void SetRelativePos(const Vec3& _vPos) { m_v3RelativePos = _vPos; SetMyUpdate(); }
    void SetRelativePosXY(const Vec2& _vPos) { m_v3RelativePos.x = _vPos.x; m_v3RelativePos.y = _vPos.y; SetMyUpdate(); }
    void SetRelativePos(float _x, float _y, float _z) { m_v3RelativePos = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativeScale(const Vec3& _vScale);
    void SetRelativeScale(float _x, float _y, float _z);

    void SetRelativeRot(const Vec3& _vRot) { m_v3RelativeRot = _vRot; SetMyUpdate(); }
    void SetRelativeRot(float _x, float _y, float _z) { m_v3RelativeRot = Vec3(_x, _y, _z); SetMyUpdate(); }
    void SetRelativeRotX(float _x) { m_v3RelativeRot.x = _x; SetMyUpdate(); }
    void SetRelativeRotY(float _y) { m_v3RelativeRot.y = _y; SetMyUpdate(); }
    void SetRelativeRotZ(float _z) { m_v3RelativeRot.z = _z; SetMyUpdate(); }

    void SetRelativePosX(float _x) { m_v3RelativePos.x = _x; SetMyUpdate(); }
    void SetRelativePosY(float _y) { m_v3RelativePos.y = _y; SetMyUpdate(); }
    void SetRelativePosZ(float _z) { m_v3RelativePos.z = _z; SetMyUpdate(); }

    void SetScaleInheritance(bool _bInherit) { m_bInheritScale = _bInherit; SetMyUpdate(); }
    void SetRotInheritance(bool _bInherit) { m_bInheritRot = _bInherit; SetMyUpdate(); }

    //상대 회전값은 변화하지만 실제로 회전하지는 않음.
    void SetLockRotation(bool _bLockRot) { m_bLockRotation = _bLockRot; }

    //이번 틱에 업데이트를 해야한다고 설정. 자신의 움직임에 영향을 받는 자식 오브젝트들에게도 재귀적으로 알림
    void SetMyUpdate();
    void SetParentUpdate() { m_bNeedParentUpdate = true; }


    //이 길이는 트랜스폼을 정사각형(또는 정육면체)라고 가정했을 때 한 모서리까지의 길이이다.
    //이 길이를 간이 충돌체 한 변의 반의 길이로 사용하면, 
    //어떤 도형이던 간에 안에 들어오는 형태의 정사각형 또는 정육면체를 만들 수 있다.(간이 충돌체로 적합)
    //사이즈 또는 스케일값이 변했을 경우 간이 충돌체 정보를 새로 생성.
    //현재 컬링 및 간이 충돌체 계산에 사용 중.
    float GetAABBSideLen() const { return m_fLongestDiagonalLen; }

    bool IsUpdated() const { return (m_bNeedMyUpdate || m_bNeedParentUpdate); }
    bool GetSizeUpdated() const { return m_bSizeUpdated; }
    bool GetDefaultScale() const { return m_bIsDefaultScale; }

    //inline Getter
    const Vec3& GetSize() const { return m_v3Size; }

    //개별 사이즈 + 월드 Scale 반영된 실제 사이즈
    Vec3 GetWorldSize() const;

    const Vec3& GetRelativePos() const { return m_v3RelativePos; }
    Vec3 GetWorldPos() const { return Vec3(m_matWorldWithoutSize.m[3]); }
    const Vec3& GetRelativeScale() const { return m_v3RelativeScale; }
    Vec3 GetWorldScale() const;
    const Vec3& GetRelativeRot() const { return m_v3RelativeRot; }
        
    Matrix GetWorldRotMat() const;  
    Vec3 GetWorldRot(eAXIS3D _eAxis) const;

    const Vec3& GetRelativeDir(eDIR_TYPE _eDir) const { return m_vRelativeDir[(UINT)_eDir]; }
    Vec3 GetWorldDir(eDIR_TYPE _eDir) const { return Vec3(m_matWorldWithoutSize.m[(UINT)_eDir]).Normalize(); }
    const Matrix& GetWorldMatWithoutSize() const { return m_matWorldWithoutSize; }
    const Matrix& GetWorldMat() const { return m_matWorld; }



    //호출 시점: CGameObject에서 finaltick() 순회 끝난 이후
    //갱신 여부를 전부 끔
    void ClearUpdateState();

private:
    void UpdateMyTransform();

    //부모의 트랜스폼 행렬을 받아서 최종적인 월드행렬을 업데이트 한다.
    void UpdateParentMatrix();
};


inline void CTransform::SetSize(const Vec3& _vSize)
{
    m_v3Size = _vSize;
    m_bSizeUpdated = true;
}

inline void CTransform::SetRelativeScale(const Vec3& _vScale)
{ 
    m_v3RelativeScale = _vScale; 
    SetMyUpdate(); 
    m_bSizeUpdated = true; 
    m_bIsDefaultScale = false;
}

inline void CTransform::SetRelativeScale(float _x, float _y, float _z)
{
    m_v3RelativeScale = Vec3(_x, _y, _z); 
    SetMyUpdate(); 
    m_bSizeUpdated = true;
    m_bIsDefaultScale = false;
}

inline void CTransform::SetMyUpdate()
{
    //이미 설정이 되어 있다면 return
    if (true == m_bNeedMyUpdate)
        return;
    m_bNeedMyUpdate = true; 
    GetOwner()->SetChildTransformToUpdate();
}
 
//inline float CTransform::GetAABBSideLen() const
//{
//    return (Vec3(m_matSize._11, m_matSize._22, m_matSize._33) * GetWorldScale()).Length();
//}

inline Vec3 CTransform::GetWorldSize() const
{
    return Vec3(m_matWorld.Right().Length(), m_matWorld.Up().Length(), m_matWorld.Front().Length());
}

inline Vec3 CTransform::GetWorldScale() const
{
    return Vec3(m_matWorldWithoutSize.Right().Length(), m_matWorldWithoutSize.Up().Length(), m_matWorldWithoutSize.Front().Length());
}

inline Matrix CTransform::GetWorldRotMat() const
{
    return Matrix(m_matWorldWithoutSize.Right().Normalize(), m_matWorldWithoutSize.Up().Normalize(), m_matWorldWithoutSize.Front().Normalize());
}

inline Vec3 CTransform::GetWorldRot(eAXIS3D _eAxis) const
{
    return m_matWorldWithoutSize.Axis((UINT)_eAxis).Normalize();
}

inline void CTransform::ClearUpdateState()
{
    m_bNeedMyUpdate = false;
    m_bNeedParentUpdate = false;
    m_bSizeUpdated = false;
}
