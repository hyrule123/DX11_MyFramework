#include "pch.h"

#include "CQuadTreeNode.h"
#include "CSpatialPartition2D.h"

#include "CCollider2D.h"

#include "CRenderMgr.h"


CQuadTreeNode::CQuadTreeNode(CSpatialPartition2D* _pOwner, CQuadTreeNode* _pParent, const tSquareInfo& _SquareInfo, int _iRecursiveLevel)
	: m_pOwner(_pOwner)
	, m_pParent(_pParent)
	, m_arrChild{}
	, m_SquareInfo(_SquareInfo)
	, m_iRecursiveLevel(_iRecursiveLevel)
{
}

CQuadTreeNode::~CQuadTreeNode()
{
}


bool CQuadTreeNode::Insert(const tColliderPartInfo& _Partition)
{
	//우선 자신의 노드 안에 들어올 수 있는지를 확인
	if (false == CheckFit(_Partition))
	{
		//루트 노드일 경우 자신보다 크더라도 일단 삽입
		if (true == IsRootNode())
		{
			m_vecCollPartInfo.push_back(_Partition);
			m_vecCollPartInfo.back().bFinal = true;
			return true;
		}

		return false;
	}

	//만약 자신의 노드에 들어올 수 있을 경우 
	if (nullptr == m_arrChild[0])
	{
		//아직 자리가 남아 있거나 재귀 단계가 끝 단계에 도달했으면 바로 삽입
		if (m_vecCollPartInfo.size() <= m_pOwner->GetCapacity() 
			|| 
			m_iRecursiveLevel >= m_pOwner->GetMaxRecursiveLevel())
		{
			m_vecCollPartInfo.push_back(_Partition);
			return true;
		}
		//그렇지 않을 경우 자식 컴포넌트를 생성하고 값을 추가적으로 더 내림
		else
		{ 
			Split();
		}
	}

	//Split이 실행된 후 이쪽으로 넘어오면 파티션을 자식노드에 넣을 수 있는지 확인한다.
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (true == m_arrChild[i]->Insert(_Partition))
			return true;
	}

	//자식노드 쪽에 추가를 시도해봤는데 모든 자식노들에 대해 들어가지 않았으면
	//최종적으로 자신의 컨테이너에 삽입하고 true를 반환한다.
	m_vecCollPartInfo.push_back(_Partition);
	m_vecCollPartInfo.back().bFinal = true;
	return true;








	////루트 노드일 경우 CheckFit에서 false가 반환되어도 강제 삽입.(대신 더이상 검사하지 않음)
	//if (false == CheckFit(_Partition))
	//{
	//	if (true == IsRootNode())
	//	{
	//		//이 경우에는 더이상 밑으로 내려 보내지 않음.
	//		_Partition.bFinal = true;
	//		m_vecCollPartInfo.push_back(_Partition);
	//		return true;
	//	}

	//	return false;
	//}

	////아직 공간이 남아있거나, 최대 재귀 레벨에 도달한 노드가 아닐 경우에는 더이상 검사하지 않고 삽입한다.
	//size_t vecsize = m_vecCollPartInfo.size();
	//if (vecsize < m_pOwner->GetCapacity() || m_iRecursiveLevel >= m_pOwner->GetMaxRecursiveLevel())
	//{
	//	m_vecCollPartInfo.push_back(_Partition);
	//	return true;
	//}
	//else
	//{
	//	//그렇지 않을 경우 자식 노드를 만들어 공간분할 정보를 자식으로 내려보낸다.
	//	Split();
	//}
	//	
	//	for (size_t i = 0; i < vecsize; i++)
	//	{
	//		Insert(m_vecCollPartInfo[i]);
	//	}
	//	m_vecCollPartInfo.clear();
}



void CQuadTreeNode::Split()
{
	float fSizeHalf = m_SquareInfo.Size * 0.5f;
	int iLevel = m_iRecursiveLevel + 1;

	//자신의 중점 기준으로 사등분해서 새로운 노드를 추가한다.
	m_arrChild[eQuadrant_1] = new CQuadTreeNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X + fSizeHalf, m_SquareInfo.LB_Y + fSizeHalf, 
			fSizeHalf },
		iLevel);

	m_arrChild[eQuadrant_2] = new CQuadTreeNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X + fSizeHalf, m_SquareInfo.LB_Y,
			fSizeHalf},
		iLevel);

	m_arrChild[eQuadrant_3] = new CQuadTreeNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X, m_SquareInfo.LB_Y, 
			fSizeHalf },
		iLevel);

	m_arrChild[eQuadrant_4] = new CQuadTreeNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X, m_SquareInfo.LB_Y + fSizeHalf, 
			fSizeHalf },
		iLevel);

	//가지고있던 데이터들을 순회돌면서 자식 컨테이너에 들어갔는지 확인
	size_t size = m_vecCollPartInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		//이미 확정된 충돌체일 경우 스킵
		if (true == m_vecCollPartInfo[i].bFinal)
			continue;

		for (int j = 0; j < eQuadrant_End; ++j)
		{
			//어떠한 자식 노드에도 넣지 못했을 경우 bFinal을 켜서 최종적으로 노드를 확정
			if (false == Insert(m_vecCollPartInfo[i]))
				m_vecCollPartInfo[i].bFinal = true;
		}

	}

	//자식 노드에 성공적으로 들어간 노드들은 제거
	m_vecCollPartInfo.erase(
		std::remove_if(m_vecCollPartInfo.begin(), m_vecCollPartInfo.end(),
		std::bind(&CQuadTreeNode::CheckNotFinal, this, std::placeholders::_1)), 
		m_vecCollPartInfo.end());
}



bool CQuadTreeNode::CheckFit(const tColliderPartInfo& _Partition)
{
	//AABB검사를 통과 못할 시 return
	if (m_SquareInfo.LB_X >= _Partition.RectInfo.LB.x + _Partition.RectInfo.Size.x)
		return false;
	if (m_SquareInfo.LB_Y >= _Partition.RectInfo.LB.y + _Partition.RectInfo.Size.y)
		return false;
	if (m_SquareInfo.LB_X + m_SquareInfo.Size <= _Partition.RectInfo.LB.x)
		return false;
	if (m_SquareInfo.LB_Y + m_SquareInfo.Size <= _Partition.RectInfo.LB.y)
		return false;

	return true;
}



void CQuadTreeNode::Collision(CCollider2D* _pColA, CCollider2D* _pColB)
{
	//두 레이어가 우선 상호작용하는지부터 검사. 상호작용 안 할 경우 continue
	if (false == m_pOwner->CheckLayerInteract(
		_pColA->GetLayerIndex(),
		_pColB->GetLayerIndex()))
		return;

	//ID를 만들고
	CollisionID ColID = {};
	UINT32 Temp = 0;
	ColID.LowID = _pColA->GetID();
	ColID.HighID = _pColB->GetID();
	if (ColID.LowID > ColID.HighID)
	{
		Temp = ColID.LowID;
		ColID.LowID = ColID.HighID;
		ColID.HighID = Temp;
	}

	//이전 프레임에 충돌했었는지를 확인
	bool PrevColl = m_pOwner->GetPrevCollided(ColID);

	//충돌이 확인되었을 경우
	if (_pColA->CheckCollision(_pColB))
	{
		//첫 충돌일 경우 BeginCollision
		if (true != PrevColl)
		{
			_pColA->BeginCollision(_pColB);

			//충돌중인 충돌체 저장 맵에 충돌정보를 등록
			tCollisionInfo Info = {};
			Info.pColliderA = _pColA;
			Info.pCOlliderB = _pColB;
			Info.bCurrent = true;
			m_pOwner->AddCollisionInfo(ColID, Info);
		}
		//이전 프레임에도 충돌 중이었을 경우
		else
		{
			//OnCollision을 호출
			_pColA->OnCollision(_pColB);

			//충돌중인 충돌체 저장 맵에서 충돌정보를 갱신
			m_pOwner->SetCollisionInfoChecked(ColID);
		}
	}
	//충돌하지 않았을 경우 + 이전 프레임에 충돌중이었을 경우
	else if(true == PrevColl)
	{
		_pColA->EndCollision(_pColB);

		//충돌중인 충돌체 저장 맵에서 충돌정보를 제거
		m_pOwner->RemoveCollisionInfo(ColID);
	}
}



void CQuadTreeNode::Destroy()
{
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->Destroy();
		delete m_arrChild[i];
	}
}

void CQuadTreeNode::DebugRender()
{
	float AbsSize = m_pOwner->GetSquareSize();
	Vec3 vSize(AbsSize, AbsSize, 1.f);
	const Matrix& matScale = Matrix::CreateScale(vSize);
	
	float SizeHalf = m_SquareInfo.Size * 0.5f;
	Vec3 vPos(m_SquareInfo.LB_X + SizeHalf, m_SquareInfo.LB_Y + SizeHalf, 0.f);
	vPos *= AbsSize;
	const Matrix& matTrans = Matrix::CreateTranslation(vPos);


	tDebugShapeInfo Info = {};
	Info.eShape = eSHAPE_RECT;
	Info.matWorld = matScale * matTrans;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록

	Info.vColor = Vec4(Vec3(0.1f, 0.1f, 0.1f) * (float)m_iRecursiveLevel, 1.f);
	CRenderMgr::GetInst()->AddDebugShapeRender(Info);

	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			break;

		m_arrChild[i]->DebugRender();
	}

}

void CQuadTreeNode::CheckCollision()
{
	CheckCollisionParent(m_vecCollPartInfo);

	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->CheckCollision();
	}
}

void CQuadTreeNode::CheckCollisionParent(const vector<tColliderPartInfo>& _ChildvecpCollider)
{
	//자식 노드로부터 올라온 벡터와 자신의 벡터를 순회 돌면서 충돌검사를 시행
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecCollPartInfo.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//같은 주소(==같은 객체)일 경우 검사하지 않는다.
			if (&(_ChildvecpCollider[C]) == &(m_vecCollPartInfo[P]))
				continue;
			Collision(_ChildvecpCollider[C].pCol, m_vecCollPartInfo[P].pCol);
		}
	}

	//더이상 부모 노드가 없을 경우 return
	if (nullptr == m_pParent)
		return;

	//쭉 타고 올라가면서 다른 부모의 리스트와 충돌 비교
	m_pParent->CheckCollisionParent(_ChildvecpCollider);
}
