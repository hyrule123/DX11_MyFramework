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

	vector<tColliderPartInfo> m_vecCollPartInfo;

	
public:
	//Getter/Setter
	tSquareInfo GetSquareInfo() const { return m_SquareInfo; }
	bool IsRootNode() const { return (nullptr == m_pParent); }


public:

	//0~1 ���̷� ����ȭ�� �浹ü ������ ���� ������.
	bool Insert(const tColliderPartInfo& _Partition);
	void Clear();
	void Destroy();
	void DebugRender();

	void CheckCollision();

	bool CheckFit(const tColliderPartInfo& _Partition);

	//�θ𿡰� �ڽ��� �浹ü ����Ʈ�� �Ѱܼ� �� Ÿ�� �ö󰣴�.
	void CheckCollisionParent(const vector<tColliderPartInfo>& _ChildvecpCollider);

private:
	void Split();
	
	bool CheckNotFinal(const tColliderPartInfo& _Partition) { return !_Partition.bFinal; }

	void Collision(CCollider2D* _pColA, CCollider2D* _pColB);
};

inline void CQuadTreeNode::Clear()
{
	m_vecCollPartInfo.clear();
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->Clear();
	}
}
