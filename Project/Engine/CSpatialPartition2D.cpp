#include "pch.h"
#include "CSpatialPartition2D.h"

#include "CQuadTreeNode.h"

#include "CCollider2D.h"


CSpatialPartition2D::CSpatialPartition2D(float _fRootNodeSize, int _iCapacity)
	: m_fRootNodeSize(_fRootNodeSize)
	, m_fSizeReserve(-1.f)
	, m_iCapacity(_iCapacity)
	, m_iMaxRecursiveLevel(3)
	, m_flagLayerInteraction{}
{
	m_fRootNodeHalfSize = 0.5f * m_fRootNodeSize;

	//��Ʈ��带 ����. ���� �־��ָ� ���Ĵ� �˾Ƽ� ��������� �ݺ���.
	//NDC ��ǥ���� ���ϴ� ��ǥ(-1, -1)���� ����, NDC ��ǥ���� ȭ�� ������(2 * 2)�� ��ġ���Ѽ� ���.
	//�� ��忡�� �߻����� '����' ���� �־����, ���⿡ ���� ����� ������ �� ���ؼ� ����ϴ� ������� ���.
	tSquareInfo Info = { -1.f, -1.f, 2.f };

	m_Root = new CQuadTreeNode(this, nullptr, Info, 0);
}


CSpatialPartition2D::~CSpatialPartition2D()
{
	m_Root->Destroy();
	delete m_Root;
}

void CSpatialPartition2D::tick()
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
	m_Root->Clear();

	if (0.f < m_fSizeReserve)
	{
		m_fRootNodeSize = m_fSizeReserve;
		m_fRootNodeHalfSize = m_fRootNodeSize / 2.f;
	}

	m_Root->DebugRender();
}

void CSpatialPartition2D::AddCollider(CCollider2D* _pCol)
{
	tColliderPartition Part = {};
	Part.pCol = _pCol;
	Part.RectInfo = _pCol->GetSpatialPartitionInfo();

	//����� Ȯ���ؾ��ϴ��� �˻�
	float absf = fabsf(Part.RectInfo.LB.x);
	if (absf > m_fRootNodeHalfSize)
		ReserveSize(absf * 2.f);

	absf = fabsf(Part.RectInfo.LB.y);
	if (absf > m_fRootNodeHalfSize)
		ReserveSize(absf * 2.f);

	absf = fabsf(Part.RectInfo.Size.x);
	if (absf > m_fRootNodeSize)
		ReserveSize(absf);

	absf = fabsf(Part.RectInfo.Size.y);
	if (absf > m_fRootNodeSize)
		ReserveSize(absf);

	Part.RectInfo.LB /= m_fRootNodeSize;
	Part.RectInfo.Size /= m_fRootNodeSize;

	m_Root->Insert(Part);
}
