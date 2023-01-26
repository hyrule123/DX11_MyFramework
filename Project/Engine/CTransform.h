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

    //��, ��, ���������� ��Ÿ���� �������� ���� ����
    //eDIR_TYPE ����ü�� ���.
    Vec3    m_vRelativeDir[eDIR_TYPE::eDIR_END];

    //���� ����(��� ȸ������ ����)
    Vec3    m_vWorldDir[eDIR_TYPE::eDIR_END];

    //�θ�κ��� ��ӹ��� ���� ����� ���
    Matrix  m_matRelative;

    //�θ�κ��� ������ Ʈ������ ����
    Matrix  m_matParent;

    bool    m_bInheritScale;
    bool    m_bInheritRot;
    
    //�θ�κ��� ��ӹ޾� ���������� ������� �������
    Matrix  m_matWorld;

    //��� ���·� ������Ʈ�� �ʿ����� ���θ� ����.
    //��ġ�� ������ �ʾҴµ� ���� ��������� ������Ʈ �� �ʿ䰡 ����.
    //m_matRelative�� ������Ʈ �ؾ��ϴ��� ���ΰ� ����Ǿ�����.
    //�ڽ��� ����� ������Ʈ �ؾ� �Ѵٸ� �ݵ�� �θ� ����� �޾ƿ;� �ϱ� ������ m_bNeedParentUpdate�� ������Ʈ ���ش�.
    bool    m_bNeedMyUpdate;

    //�ڽ��� �ƴ϶� �θ���� ����� ������Ʈ�Ǿ��� ��� - �ڽ��� ��������� ������Ʈ�� �ʿ䰡 ����.
    bool    m_bNeedParentUpdate;

    //Size �Ǵ� Scale�� ����Ǿ��� ��� �� ���� Ȱ��ȭ ��Ű�� finaltick���� ����Ѵ�.
    bool    m_bNeedAABBUpdate;

    //�� ���̴� Ʈ�������� ���簢��(�Ǵ� ������ü)��� �������� �� �� �𼭸������� �����̴�.
    //�� ���̸� ���� �浹ü �� ���� ���� ���̷� ����ϸ�, 
    //� �����̴� ���� �ȿ� ������ ������ ���簢�� �Ǵ� ������ü�� ���� �� �ִ�.(���� �浹ü�� ����)
    float   m_fAABBSideLenHalf;

public:
    //inline Setter

    //�������� ��� ��ü�� ������ �ʿ� ����.
    void SetSize(const Vec3& _vSize);

    void SetRelativePos(const Vec3& _vPos) { m_vRelativePos = _vPos; SetMyUpdate(); }
    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativeScale(const Vec3& _vScale) { m_vRelativeScale = _vScale; SetMyUpdate(); m_bNeedAABBUpdate = true; }
    void SetRelativeScale(float _x, float _y, float _z);

    void SetRelativeRot(const Vec3& _vRot) { m_vRelativeRot = _vRot; SetMyUpdate(); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); SetMyUpdate(); }

    void SetRelativePosX(float _x) { m_vRelativePos.x = _x; SetMyUpdate(); }
    void SetRelativePosY(float _y) { m_vRelativePos.y = _y; SetMyUpdate(); }
    void SetRelativePosZ(float _z) { m_vRelativePos.z = _z; SetMyUpdate(); }

    void SetScaleInheritance(bool _bInherit) { m_bInheritScale = _bInherit; SetMyUpdate(); }
    void SetRotInheritance(bool _bInherit) { m_bInheritRot = _bInherit; SetMyUpdate(); }

    //�̹� ƽ�� ������Ʈ�� �ؾ��Ѵٰ� ����. �ڽ��� �����ӿ� ������ �޴� �ڽ� ������Ʈ�鿡�Ե� ��������� �˸�
    void SetMyUpdate();
    void SetParentUpdate() { m_bNeedParentUpdate = true; }

    //������ �Ǵ� �����ϰ��� ������ ��� ���� �浹ü ������ ���� ����.
    void CalcAABB();

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

    float GetAABBSideLen() const { return m_fAABBSideLenHalf; }

private:
    void UpdateMyTransform();

    //�θ��� Ʈ������ ����� �޾Ƽ� �������� ��������� ������Ʈ �Ѵ�.
    void UpdateParentMatrix();

public:
    virtual void finaltick() override;    
    virtual void cleanup() override {};
    void UpdateData();


    CLONE(CTransform);    
public:
    CTransform();
    //�ܼ� Value�� ���� ���̹Ƿ� �⺻ ���� �����ڷε� �����.
    ~CTransform();
};


inline void CTransform::SetSize(const Vec3& _vSize)
{
    m_matSize._11 = _vSize.x; m_matSize._22 = _vSize.y; m_matSize._33 = _vSize.z;
    m_bNeedAABBUpdate = true;
}

inline void CTransform::SetRelativeScale(float _x, float _y, float _z)
{
    m_vRelativeScale = Vec3(_x, _y, _z); SetMyUpdate(); 
    m_bNeedAABBUpdate = true;
}

inline void CTransform::SetMyUpdate()
{
    //�̹� ������ �Ǿ� �ִٸ� return
    if (true == m_bNeedMyUpdate)
        return;
    m_bNeedMyUpdate = true; 
    GetOwner()->SetChildTransformToUpdate();
}

inline void CTransform::CalcAABB()
{
    m_fAABBSideLenHalf = (Vec3(m_matSize._11, m_matSize._22, m_matSize._33) * GetWorldScale()).Length();
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
