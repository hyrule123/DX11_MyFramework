#pragma once

#include "CSpatialPartition.h"



union CollisionID
{
	struct
	{
		UINT32 LowID;  //충돌체 중 ID 숫자가 작은 충돌체의 ID
		UINT32 HighID; //충돌체 중 ID 숫자가 큰 충돌체의 ID
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

	//가장 큰 사각형(루트 노드)의 한 변의 사이즈
	float				m_fRootNodeSize;
	float				m_fRootNodeHalfSize;
	float				m_fSizeReserve;

	int					m_iCapacity;

	//재귀 반복 한계치. 이 이상으로 자식 노드를 생성하지 못하도록 막는 용도.
	int			m_iMaxRecursiveLevel;

	unordered_map<UINT64, tCollisionInfo> m_umapColliding;
	UINT32 m_flagLayerInteraction[MAX_LAYER];
public://Inline Methods
	void ReserveSize(float _size) { if (m_fSizeReserve < _size) { m_fSizeReserve = _size; } }
	int GetCapacity() { return m_iCapacity; }
	float GetSquareSize() const { return m_fRootNodeSize; }
	int GetMaxRecursiveLevel() const { return m_iMaxRecursiveLevel; }

	bool GetPrevCollided(CollisionID _ColID);

	//첫 충돌 시 등록
	void AddCollisionInfo(CollisionID _ColID, const tCollisionInfo& _ColInfo);

	void SetCollisionInfoChecked(CollisionID _ColID);

	//충돌 종료시 제거
	void RemoveCollisionInfo(CollisionID _ColID);

	//레이어, ID 관련 함수
	//오름차순으로 정렬
	void SortAscending(UINT32& _uiLow, UINT32& _uiHigh);

	//두 레이어가 상호작용하도록 등록 되어있는지 확인.
	bool CheckLayerInteract(UINT32 _iLowLayer, UINT32 _iHighLayer);
	//두 레이어의 상호작용을 등록
	void AddLayerInteract(UINT32 _iLowLayer, UINT32 _iHighLayer);
	//한 레이어가 모든 레이어(자신 포함) 상호작용하도록 등록.
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
	//행렬의 형태이므로 대칭되는 형태로 비트플래그를 등록.
	//상호작용의 경우 여러번 호출되는 함수가 아니기 때문에 오히려 여기서 두번 호출하는게 편할 듯.
	//대칭되는 형태로 등록하지 않으면 매번 검사할때마다 크기 판별을 해야 하는데 그게 더 비효율적일 것 같음
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
