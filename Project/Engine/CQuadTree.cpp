#include "pch.h"
#include "CQuadTree.h"

#include "CQuadNode.h"

#include "CCollider2D.h"


CQuadTree::CQuadTree(float _fRootNodeSize, int _iCapacity)
	: m_fRootNodeSize(_fRootNodeSize)
	, m_fReserveResize(-1.f)
	, m_iCapacity(_iCapacity)
	, m_iMaxRecursiveLevel(10)
	, m_flagLayerInteraction{}
{
	m_fRootNodeHalfSize = 0.5f * m_fRootNodeSize;

	//��Ʈ��带 ����. ���� �־��ָ� ���Ĵ� �˾Ƽ� ��������� �ݺ���.
	//NDC ��ǥ���� ���ϴ� ��ǥ(-1, -1)���� ����, NDC ��ǥ���� ȭ�� ������(2 * 2)�� ��ġ���Ѽ� ���.
	//�� ��忡�� �߻����� '����' ���� �־����, ���⿡ ���� ����� ������ �� ���ؼ� ����ϴ� ������� ���.
	tSquareInfo Info = { -m_fRootNodeHalfSize, -m_fRootNodeHalfSize, m_fRootNodeSize };

	m_Root = new CQuadNode(this, nullptr, Info, 0);
}


CQuadTree::~CQuadTree()
{
	m_Root->Destroy();
	delete m_Root;
}

void CQuadTree::tick()
{
	//unordered map�� bCurrent�� ��� false�� ������ �ְ�
	for (auto& iter : m_umapColliding)
	{
		iter.second.bCurrent = false;
	}

	//�浹 �˻縦 �ǽ��� ��
	m_Root->CheckCollision();

	//bCurrent�� �ٲ��� ���� Ŭ�����鿡 ���� �浹 ���� ó��
	auto iter = m_umapColliding.begin();
	const auto& iterEnd = m_umapColliding.end();
	while (iter != iterEnd)
	{
		//�̹��� �浹ó���� ������� �ʾҴٴ� ���� ���� �����ӿ� �浹������ ���� �浹���� �ƴ϶�� ����
		//�浹 ���� ó�� �� map���� ���Ÿ� ���ش�.
		if (false == iter->second.bCurrent)
		{
			iter->second.pColliderA->EndCollision(iter->second.pCOlliderB);
			iter = m_umapColliding.erase(iter);
		}
		else
			++iter;
	}

	//����Ʈ�� ���� �����͸� ��� ����
	//m_Root->Clear();

	if (0.f < m_fReserveResize)
	{
		m_fRootNodeSize = m_fReserveResize;
		m_fRootNodeHalfSize = m_fRootNodeSize * 0.5f;
		m_fReserveResize = -1.f;
	}

	m_Root->DebugRender();
}

void CQuadTree::AddCollider(CCollider2D* _pCol)
{
	tColliderPartInfo Part = {};
	Part.pCol = _pCol;
	Part.RectInfo = _pCol->GetSpatialPartitionInfo();

	m_Root->Insert(Part);
}
