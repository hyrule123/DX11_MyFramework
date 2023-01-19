#pragma once

#include "global.h"
#include "CSpatialPartitionNode.h"

enum eQuadrant
{
	eQuadrant_1,
	eQuadrant_2,
	eQuadrant_3,
	eQuadrant_4,
	eQuadrant_End
};



class CQuadTree;
class CQuadNode
	:public CSpatialPartitionNode
{
private:
	CQuadNode() = delete;
public:
	CQuadNode(CQuadTree* _pOwner, CQuadNode* _pParent, const tSquareInfo& _SquareInfo, int _iRecursiveLevel);
	~CQuadNode();

private:
	CQuadTree* m_pOwner;
	CQuadNode* m_pParent;
	CQuadNode* m_arrChild[eQuadrant_End];

	tSquareInfo m_SquareInfo;

	int m_iRecursiveLevel;

	vector<tColliderPartInfo> m_vecCollPartInfo;

	int m_iMaxCapacity;
	int m_iMaxRecursiveLevel;

	
public:
	//Getter/Setter
	tSquareInfo GetSquareInfo() const { return m_SquareInfo; }
	bool IsRootNode() const { return (nullptr == m_pParent); }


public:

	//0~1 사이로 정규화된 충돌체 정보가 들어올 예정임.
	bool Insert(const tColliderPartInfo& _Partition);
	void Clear();
	void Destroy();
	void DebugRender();

	void CheckCollision();

	bool CheckFit(const tColliderPartInfo& _Partition);

	//부모에게 자신의 충돌체 리스트를 넘겨서 쭉 타고 올라간다.
	void CheckCollisionParent(vector<tColliderPartInfo>& _ChildvecpCollider);

private:
	void Split();
	
	bool CheckNotFinal(const tColliderPartInfo& _Partition) { return !_Partition.bFinal; }

	void Collision(CCollider2D* _pColA, CCollider2D* _pColB);
};

inline void CQuadNode::Clear()
{
	m_vecCollPartInfo.clear();
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->Clear();
	}
}

inline bool CQuadNode::CheckFit(const tColliderPartInfo& _Partition)
{
	//AABB검사를 통과 못할 시 return
	if (
		m_SquareInfo.LB_X < _Partition.RectInfo.LB.x
		&&
		m_SquareInfo.LB_Y < _Partition.RectInfo.LB.y
		&&
		m_SquareInfo.LB_X + m_SquareInfo.Size > _Partition.RectInfo.RT.x
		&&
		m_SquareInfo.LB_Y + m_SquareInfo.Size > _Partition.RectInfo.RT.y
		)
		return true;

	return false;
}