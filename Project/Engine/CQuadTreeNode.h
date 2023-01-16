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



class CSpatialPartition2D;
class CQuadTreeNode
	:public CSpatialPartitionNode
{
private:
	CQuadTreeNode() = delete;
public:
	CQuadTreeNode(CSpatialPartition2D* _pOwner, CQuadTreeNode* _pParent, const tSquareInfo& _SquareInfo, int _iRecursiveLevel);
	~CQuadTreeNode();

private:
	CSpatialPartition2D* m_pOwner;
	CQuadTreeNode* m_pParent;
	CQuadTreeNode* m_arrChild[eQuadrant_End];

	tSquareInfo m_SquareInfo;

	int m_iRecursiveLevel;

	vector<tColliderPartition> m_vecpCollider;

	
public:
	//Getter/Setter
	tSquareInfo GetSquareInfo() const { return m_SquareInfo; }
	


public:
	//0~1 사이로 정규화된 충돌체 정보가 들어올 예정임.
	void Insert(tColliderPartition& _Partition);
	bool CheckChildFIt(tColliderPartition& _Partition)
	void Clear();
	void Destroy();
	void DebugRender();

	void CheckCollision();

	//부모에게 자신의 충돌체 리스트를 넘겨서 쭉 타고 올라간다.
	void CheckCollisionParent(const vector<tColliderPartition>& _ChildvecpCollider);

private:
	void Split();

	void Collision(CCollider2D* _pColA, CCollider2D* _pColB);
};

inline void CQuadTreeNode::Clear()
{
	m_vecpCollider.clear();
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->Clear();
	}
}