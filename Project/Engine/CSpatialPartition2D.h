#pragma once

#include "CSpatialPartition.h"



union CollisionID
{
	struct
	{
		UINT32 LowID;  //�浹ü �� ID ���ڰ� ���� �浹ü�� ID
		UINT32 HighID; //�浹ü �� ID ���ڰ� ū �浹ü�� ID
	};

	UINT64 FullID;
};

class CCollider;
class CCollider2D;
struct tCollisionInfo
{
	CCollider* pColliderA;
	CCollider* pCOlliderB;
	bool       bCurrent;
};


class CQuadTreeNode;

class CSpatialPartition2D
	: public CSpatialPartition
{
private:
	CSpatialPartition2D() = delete;
public:
	CSpatialPartition2D(float _fRootNodeSize, int _iCapacity);
	~CSpatialPartition2D();

private:
	CQuadTreeNode*		m_Root;

	//���� ū �簢��(��Ʈ ���)�� �� ���� ������
	float				m_fRootNodeSize;
	float				m_fRootNodeHalfSize;
	float				m_fReserveResize;

	int					m_iCapacity;

	//��� �ݺ� �Ѱ�ġ. �� �̻����� �ڽ� ��带 �������� ���ϵ��� ���� �뵵.
	int			m_iMaxRecursiveLevel;

	unordered_map<UINT64, tCollisionInfo> m_umapColliding;
	UINT32 m_flagLayerInteraction[MAX_LAYER];
public://Inline Methods
	void ReserveResize(float _size) { if (m_fReserveResize < _size) { m_fReserveResize = _size; } }
	int GetCapacity() { return m_iCapacity; }
	float GetSquareSize() const { return m_fRootNodeSize; }
	int GetMaxRecursiveLevel() const { return m_iMaxRecursiveLevel; }

	bool GetPrevCollided(CollisionID _ColID);

	//ù �浹 �� ���
	void AddCollisionInfo(CollisionID _ColID, const tCollisionInfo& _ColInfo);

	void SetCollisionInfoChecked(CollisionID _ColID);

	//�浹 ����� ����
	void RemoveCollisionInfo(CollisionID _ColID);

	//���̾�, ID ���� �Լ�
	//������������ ����
	void SortAscending(UINT32& _uiLow, UINT32& _uiHigh);

	//�� ���̾ ��ȣ�ۿ��ϵ��� ��� �Ǿ��ִ��� Ȯ��.
	bool CheckLayerInteract(UINT32 _iLowLayer, UINT32 _iHighLayer);
	//�� ���̾��� ��ȣ�ۿ��� ���
	void AddLayerInteract(UINT32 _iLowLayer, UINT32 _iHighLayer);
	//�� ���̾ ��� ���̾�(�ڽ� ����) ��ȣ�ۿ��ϵ��� ���.
	void AddLayerInteractAll(UINT32 _iLayer) { m_flagLayerInteraction[_iLayer] = UINT32_MAX; }

	void RemoveLayerInteract(UINT32 _iLowLayer, UINT32 _iHighLayer);
	void RemoveLayerInteractAll(UINT32 _iLowLayer);

public:
	void AddCollider(CCollider2D* _pCol);

public://Logic
	void tick();
};



inline bool CSpatialPartition2D::GetPrevCollided(CollisionID _ColID)
{
	const auto& iter = m_umapColliding.find(_ColID.FullID);
	if (iter == m_umapColliding.end())
		return false;

	return true;
}

inline void CSpatialPartition2D::AddCollisionInfo(CollisionID _ColID, const tCollisionInfo& _ColInfo)
{
	m_umapColliding.insert(make_pair(_ColID.FullID, _ColInfo));
}

inline void CSpatialPartition2D::SetCollisionInfoChecked(CollisionID _ColID)
{
	const auto& iter = m_umapColliding.find(_ColID.FullID);
	if (iter == m_umapColliding.end())
		return;

	iter->second.bCurrent = true;
}

inline void CSpatialPartition2D::RemoveCollisionInfo(CollisionID _ColID)
{
	const auto& iter = m_umapColliding.find(_ColID.FullID);

	if (iter == m_umapColliding.end())
		return;

	m_umapColliding.erase(iter);
}


inline void CSpatialPartition2D::SortAscending(UINT32& _uiLow, UINT32& _uiHigh)
{
	if (_uiLow <= _uiHigh)
		return;

	UINT32 Temp = _uiLow;
	_uiLow = _uiHigh;
	_uiHigh = Temp;
}

inline bool CSpatialPartition2D::CheckLayerInteract(UINT32 _iLayer1, UINT32 _iLayer2)
{
	return (m_flagLayerInteraction[_iLayer1] & (1 << _iLayer2));
}


inline void CSpatialPartition2D::AddLayerInteract(UINT32 _iLayer1, UINT32 _iLayer2)
{
	//����� �����̹Ƿ� ��Ī�Ǵ� ���·� ��Ʈ�÷��׸� ���.
	//��ȣ�ۿ��� ��� ������ ȣ��Ǵ� �Լ��� �ƴϱ� ������ ������ ���⼭ �ι� ȣ���ϴ°� ���� ��.
	//��Ī�Ǵ� ���·� ������� ������ �Ź� �˻��Ҷ����� ũ�� �Ǻ��� �ؾ� �ϴµ� �װ� �� ��ȿ������ �� ����
	m_flagLayerInteraction[_iLayer1] |= 1 << _iLayer2;
	m_flagLayerInteraction[_iLayer2] |= 1 << _iLayer1;
}

inline void CSpatialPartition2D::RemoveLayerInteract(UINT32 _iLayer1, UINT32 _iLayer2)
{
	m_flagLayerInteraction[_iLayer1] &= ~(1 << _iLayer2);
}

inline void CSpatialPartition2D::RemoveLayerInteractAll(UINT32 _iLayer)
{
	m_flagLayerInteraction[_iLayer] = (UINT32)0;
}
