#pragma once
#include "CComponent.h"


class CTransform :
    public CComponent
{
public:
    CTransform();
    //�ܼ� Value�� ���� ���̹Ƿ� �⺻ ���� �����ڷε� �����.
    ~CTransform();
    CLONE(CTransform);

public:
    virtual void finaltick() override;
    virtual void cleanup() override {};
    //void UpdateData();


private:
    Matrix    m_matSize;

    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    float m_fLongestDiagonalLen;

    //��, ��, ���������� ��Ÿ���� �������� ���� ����
    //eDIR_TYPE ����ü�� ���.
    Vec3    m_vRelativeDir[(UINT)eDIR_TYPE::END];

    //���� ����(��� ȸ������ ����)
    Vec3    m_vWorldDir[(UINT)eDIR_TYPE::END];

    //�θ�κ��� ��ӹ��� ���� ����� ���
    Matrix  m_matRelative;

    //�θ�κ��� ������ Ʈ������ ����
    Matrix  m_matParent;

    bool    m_bInheritScale;
    bool    m_bInheritRot;
    bool    m_bLockRot; //�ڽ��� ȸ�� ����
    
    //�θ�κ��� ��ӹ޾� ���������� ������� �������
    Matrix  m_matWorld;

    //��� ���·� ������Ʈ�� �ʿ����� ���θ� ����.
    //��ġ�� ������ �ʾҴµ� ���� ��������� ������Ʈ �� �ʿ䰡 ����.
    //m_matRelative�� ������Ʈ �ؾ��ϴ��� ���ΰ� ����Ǿ�����.
    //�ڽ��� ����� ������Ʈ �ؾ� �Ѵٸ� �ݵ�� �θ� ����� �޾ƿ;� �ϱ� ������ m_bNeedParentUpdate�� ������Ʈ ���ش�.
    bool    m_bNeedMyUpdate;

    //�ڽ��� �ƴ϶� �θ���� ����� ������Ʈ�Ǿ��� ��� - �ڽ��� ��������� ������Ʈ�� �ʿ䰡 ����.
    bool    m_bNeedParentUpdate;

    //Size�� �ڽſ��Ը� ����Ǵ� �������̹Ƿ� ��������� ������ �ʿ� ����.
    bool    m_bSizeUpdated;

public:
    //inline Setter

    //�������� ��� ��ü�� ������ �ʿ� ����.
    void SetSize(const Vec3& _vSize);

    void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; SetMyUpdate(); }
    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; SetMyUpdate(); m_bSizeUpdated = true; }
    void SetRelativeScale(float _x, float _y, float _z);

    void SetRelativeRot(const Vec3& _vRot) { m_vRelativeRot = _vRot; SetMyUpdate(); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativePosX(float _x) { m_vRelativePos.x = _x; SetMyUpdate(); }
    void SetRelativePosY(float _y) { m_vRelativePos.y = _y; SetMyUpdate(); }
    void SetRelativePosZ(float _z) { m_vRelativePos.z = _z; SetMyUpdate(); }

    void SetScaleInheritance(bool _bInherit) { m_bInheritScale = _bInherit; SetMyUpdate(); }
    void SetRotInheritance(bool _bInherit) { m_bInheritRot = _bInherit; SetMyUpdate(); }

    //��� ȸ������ ��ȭ������ ������ ȸ�������� ����.
    void SetLockRotation(bool _bLockRot) { m_bLockRot = _bLockRot; }

    //�̹� ƽ�� ������Ʈ�� �ؾ��Ѵٰ� ����. �ڽ��� �����ӿ� ������ �޴� �ڽ� ������Ʈ�鿡�Ե� ��������� �˸�
    void SetMyUpdate();
    void SetParentUpdate() { m_bNeedParentUpdate = true; }


    //�� ���̴� Ʈ�������� ���簢��(�Ǵ� ������ü)��� �������� �� �� �𼭸������� �����̴�.
    //�� ���̸� ���� �浹ü �� ���� ���� ���̷� ����ϸ�, 
    //� �����̴� ���� �ȿ� ������ ������ ���簢�� �Ǵ� ������ü�� ���� �� �ִ�.(���� �浹ü�� ����)
    //������ �Ǵ� �����ϰ��� ������ ��� ���� �浹ü ������ ���� ����.
    float GetAABBSideLen() const { return m_fLongestDiagonalLen; }

    bool IsUpdated() const { return (m_bNeedMyUpdate || m_bNeedParentUpdate); }
    bool GetSizeUpdated() const { return m_bSizeUpdated; }

    //inline Getter
    ////Inline methods don't need to return the value by const reference
    Vec3 GetSize() const { return Vec3(m_matSize._11, m_matSize._22, m_matSize._33); }
    const Matrix& GetMatSize() const { return m_matSize; }
    const Vec3& GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetWorldPos() const { return Vec3(m_matWorld.m[3]); }
    const Vec3& GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetWorldScale() const;
    const Vec3& GetRelativeRot() const { return m_vRelativeRot; }
        
    Matrix GetWorldRotMat() const;  
    Vec3 GetWorldRot(eAXIS3D _eAxis) const;

    const Vec3& GetRelativeDir(eDIR_TYPE _eDir) const { return m_vRelativeDir[(UINT)_eDir]; }
    Vec3 GetWorldDir(eDIR_TYPE _eDir) const { return Vec3(m_matWorld.m[(UINT)_eDir]).Normalize(); }
    const Matrix& GetWorldMatWithoutSize() const { return m_matWorld; }



    //ȣ�� ����: CGameObject���� finaltick() ��ȸ ���� ����
    //���� ���θ� ���� ��
    void ClearUpdateState();

private:
    void UpdateMyTransform();

    //�θ��� Ʈ������ ����� �޾Ƽ� �������� ��������� ������Ʈ �Ѵ�.
    void UpdateParentMatrix();

};


inline void CTransform::SetSize(const Vec3& _vSize)
{
    m_matSize._11 = _vSize.x; m_matSize._22 = _vSize.y; m_matSize._33 = _vSize.z;
    m_bSizeUpdated = true;
}

inline void CTransform::SetRelativeScale(float _x, float _y, float _z)
{
    m_vRelativeScale = Vec3(_x, _y, _z); 
    SetMyUpdate(); 
    m_bSizeUpdated = true;
}

inline void CTransform::SetMyUpdate()
{
    //�̹� ������ �Ǿ� �ִٸ� return
    if (true == m_bNeedMyUpdate)
        return;
    m_bNeedMyUpdate = true; 
    GetOwner()->SetChildTransformToUpdate();
}
 
//inline float CTransform::GetAABBSideLen() const
//{
//    return (Vec3(m_matSize._11, m_matSize._22, m_matSize._33) * GetWorldScale()).Length();
//}

inline Vec3 CTransform::GetWorldScale() const
{
    return Vec3(m_matWorld.Right().Length(), m_matWorld.Up().Length(), m_matWorld.Front().Length());
}

inline Matrix CTransform::GetWorldRotMat() const
{
    return Matrix(m_matWorld.Right().Normalize(), m_matWorld.Up().Normalize(), m_matWorld.Front().Normalize());
}

inline Vec3 CTransform::GetWorldRot(eAXIS3D _eAxis) const
{
    return m_matWorld.Axis((UINT)_eAxis).Normalize();
}

inline void CTransform::ClearUpdateState()
{
    m_bNeedMyUpdate = false;
    m_bNeedParentUpdate = false;
    m_bSizeUpdated = false;
}