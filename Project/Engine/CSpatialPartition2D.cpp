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

	//루트노드를 생성. 값을 넣어주면 이후는 알아서 재귀적으로 반복됨.
	//NDC 좌표계의 좌하단 좌표(-1, -1)에서 시작, NDC 좌표계의 화면 사이즈(2 * 2)와 일치시켜서 사용.
	//각 노드에는 추상적인 '배율' 값만 넣어놓고, 여기에 실제 사이즈를 저장한 뒤 곱해서 사용하는 방식으로 계산.
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
	//unordered map의 bCurrent를 모두 false로 변경해 주고
	for (auto& iter : m_umapColliding)
	{
		iter.second.bCurrent = false;
	}

	//충돌 검사를 실시한 뒤
	m_Root->CheckCollision();

	//bCurrent가 바뀌지 않은 클래스들에 대해 충돌 해제 처리
	auto iter = m_umapColliding.begin();
	const auto& iterEnd = m_umapColliding.end();
	while (iter != iterEnd)
	{
		//이번에 충돌처리가 진행되지 않았다는 것은 이전 프레임에 충돌했지만 현재 충돌중이 아니라는 것임
		//충돌 해제 처리 후 map에서 제거를 해준다.
		if (false == iter->second.bCurrent)
		{
			iter->second.pColliderA->EndCollision(iter->second.pCOlliderB);
			iter = m_umapColliding.erase(iter);
		}
		else
			++iter;
	}

	//쿼드트리 내의 데이터를 모두 제거
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

	//사이즈를 확장해야하는지 검사
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
