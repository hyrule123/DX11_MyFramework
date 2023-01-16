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


void CQuadTreeNode::Insert(tColliderPartition& _Partition)
{
	if (nullptr == m_arrChild[0])
	{
		//일단 데이터를 추가한다.
		m_vecpCollider.push_back(_Partition);

		//Capacity 이상으로 데이터가 차있고, 최대 재귀 수준까지 도달하지 않았다면 하위 노드를 생성해서 데이터를 내려보낸다.
		if ((m_vecpCollider.size() > m_pOwner->GetCapacity()) && m_pOwner->GetMaxRecursiveLevel() > m_iRecursiveLevel)
		{
			//하위 노드 생성 및 하위 노드로 데이터를 내려 보내는 메소드
			Split();
		}
	}

	//여긴 자식 노드가 있을 경우 진입하게 되는 코드이다.
	//자식 노드들을 순회돌면서 추가적으로 줄여나갈 수 있는지 확인한다.
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		const tSquareInfo& ChildInfo = m_arrChild[i]->GetSquareInfo();
		if (ChildInfo.LB_X >= _Partition.RectInfo.LB.x + _Partition.RectInfo.Size.x)
			continue;
		if (ChildInfo.LB_Y >= _Partition.RectInfo.LB.y + _Partition.RectInfo.Size.y)
			continue;
		if (ChildInfo.LB_X + ChildInfo.Size <= _Partition.RectInfo.LB.x)
			continue;
		if (ChildInfo.LB_Y + ChildInfo.Size <= _Partition.RectInfo.LB.y)
			continue;

		//조건에 맞는 자식 노드가 있을 경우 거기에 재귀적으로 삽입한다.
		m_arrChild[i]->Insert(_Partition);
		return;
	}
	
	//모든 자식 노드에 들어가지 않았을 경우 여기에 삽입.
	_Partition.bFinal = true;
	m_vecpCollider.push_back(_Partition);
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

	
	size_t size = m_vecpCollider.size();
	for (size_t i = 0; i < size; ++i)
	{

	}




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
	float Size = m_pOwner->GetSquareSize();
	float SizeHalf = Size * 0.5f;
	Vec3 vPos(m_SquareInfo.LB_X * Size + SizeHalf, m_SquareInfo.LB_Y * Size + SizeHalf, 0.f);
	const Matrix& matTrans = Matrix::CreateTranslation(vPos);

	Vec3 vSize(Size, Size, 1.f);
	const Matrix& matScale = Matrix::CreateScale(vSize);

	tDebugShapeInfo Info = {};
	Info.eShape = eSHAPE_RECT;
	Info.matWorld = matScale * matTrans;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	Info.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
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
	CheckCollisionParent(m_vecpCollider);

	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			return;

		m_arrChild[i]->CheckCollision();
	}
}

void CQuadTreeNode::CheckCollisionParent(const vector<tColliderPartition>& _ChildvecpCollider)
{
	//자식 노드로부터 올라온 벡터와 자신의 벡터를 순회 돌면서 충돌검사를 시행
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecpCollider.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//같은 주소(==같은 객체)일 경우 검사하지 않는다.
			if (&(_ChildvecpCollider[C]) == &(m_vecpCollider[P]))
				continue;
			Collision(_ChildvecpCollider[C].pCol, m_vecpCollider[P].pCol);
		}
	}

	//더이상 부모 노드가 없을 경우 return
	if (nullptr == m_pParent)
		return;

	//쭉 타고 올라가면서 다른 부모의 리스트와 충돌 비교
	m_pParent->CheckCollisionParent(_ChildvecpCollider);
}
