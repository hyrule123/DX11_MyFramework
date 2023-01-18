#include "pch.h"

#include "CQuadNode.h"
#include "CQuadTree.h"

#include "CCollider2D.h"

#include "CRenderMgr.h"


CQuadNode::CQuadNode(CQuadTree* _pOwner, CQuadNode* _pParent, const tSquareInfo& _SquareInfo, int _iRecursiveLevel)
	: m_pOwner(_pOwner)
	, m_pParent(_pParent)
	, m_arrChild{}
	, m_SquareInfo(_SquareInfo)
	, m_iRecursiveLevel(_iRecursiveLevel)
{
}

CQuadNode::~CQuadNode()
{
}


bool CQuadNode::Insert(const tColliderPartInfo& _Partition)
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
}



void CQuadNode::Split()
{
	float fSizeHalf = m_SquareInfo.Size * 0.5f;
	int iLevel = m_iRecursiveLevel + 1;

	//자신의 중점 기준으로 사등분해서 새로운 노드를 추가한다.
	m_arrChild[eQuadrant_1] = new CQuadNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X + fSizeHalf, m_SquareInfo.LB_Y + fSizeHalf, 
			fSizeHalf },
		iLevel);

	m_arrChild[eQuadrant_2] = new CQuadNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X + fSizeHalf, m_SquareInfo.LB_Y,
			fSizeHalf},
		iLevel);

	m_arrChild[eQuadrant_3] = new CQuadNode(m_pOwner, this,
		tSquareInfo{ 
			m_SquareInfo.LB_X, m_SquareInfo.LB_Y, 
			fSizeHalf },
		iLevel);

	m_arrChild[eQuadrant_4] = new CQuadNode(m_pOwner, this,
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
			//자식 노드에 들어갔을 경우 다음 충돌체를 진행
			if (true == m_arrChild[j]->Insert(m_vecCollPartInfo[i]))
				break;
		}

		//for문에서 break되지 못하고 여기까지 옴 == 사이즈가 맞는 자식 노드가 없음->final on
		m_vecCollPartInfo[i].bFinal = true;
	}

	//자식 노드에 성공적으로 들어간 노드들은 제거
	m_vecCollPartInfo.erase(
		std::remove_if(m_vecCollPartInfo.begin(), m_vecCollPartInfo.end(),
		std::bind(&CQuadNode::CheckNotFinal, this, std::placeholders::_1)), 
		m_vecCollPartInfo.end());
}



bool CQuadNode::CheckFit(const tColliderPartInfo& _Partition)
{
	//AABB검사를 통과 못할 시 return
	if (m_SquareInfo.LB_X >= _Partition.RectInfo.LB.x)
		return false;
	if (m_SquareInfo.LB_Y >= _Partition.RectInfo.LB.y)
		return false;
	if (m_SquareInfo.LB_X + m_SquareInfo.Size <= _Partition.RectInfo.RT.x)
		return false;
	if (m_SquareInfo.LB_Y + m_SquareInfo.Size <= _Partition.RectInfo.RT.y)
		return false;

	return true;
}



void CQuadNode::Collision(CCollider2D* _pColA, CCollider2D* _pColB)
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
	//충돌하지 않았을 경우 + 이전 프레임에 충돌중이었을 경우 = 충돌 해제
	else if(true == PrevColl)
	{
		_pColA->EndCollision(_pColB);

		//충돌중인 충돌체 저장 맵에서 충돌정보를 제거
		m_pOwner->RemoveCollisionInfo(ColID);
	}
}



void CQuadNode::Destroy()
{
	if (nullptr == m_arrChild[0])
		return;
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		m_arrChild[i]->Destroy();
		delete m_arrChild[i];
	}
}

void CQuadNode::DebugRender()
{
	//자신의 한 변의 길이 = 크기
	float RootSideLen = m_pOwner->GetSquareSize();
	float mySideLen = RootSideLen * m_SquareInfo.Size;
	Vec3 vSize(mySideLen, mySideLen, 1.f);
	const Matrix& matScale = Matrix::CreateScale(vSize);
	
	//자심의 시작점
	float SizeHalf = m_SquareInfo.Size * 0.5f;
	Vec3 vPos(m_SquareInfo.LB_X + SizeHalf, m_SquareInfo.LB_Y + SizeHalf, 0.f);

	//시작점도 루트 노드의 한 변의 길이에 곱해서 위치를 알아낸다.
	vPos *= RootSideLen;
	const Matrix& matTrans = Matrix::CreateTranslation(vPos);


	tDebugShapeInfo Info = {};
	Info.eShape = eSHAPE_RECT;
	Info.matWorld = matScale * matTrans;

	//충돌 중인 물체가 있을 경우 빨강, 아닐 경우 초록
	int remainder = m_iRecursiveLevel % 4;
	switch (remainder)
	{
	case 0 :
		Info.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		break;
	case 1:
		Info.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		break;
	case 2:
		Info.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
		break;
	case 3:
		Info.vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		break;
	default:
		Info.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		break;
	}

	CRenderMgr::GetInst()->AddDebugShapeRender(Info);

	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (nullptr == m_arrChild[i])
			break;

		m_arrChild[i]->DebugRender();
	}

}

void CQuadNode::CheckCollision()
{
	//자신의 CheckCollisionParent를 호출. 자신부터 시작해서 루트 노드까지 타고 올라가면서 모든 충돌체와 충돌을 검사.
	CheckCollisionParent(m_vecCollPartInfo);

	//자식노드가 없을경우 return
	if (nullptr == m_arrChild[0])
		return;
	//자식 노드들도 CheckCollidion 호출
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		m_arrChild[i]->CheckCollision();
	}
}

void CQuadNode::CheckCollisionParent(const vector<tColliderPartInfo>& _ChildvecpCollider)
{
	//자식 노드로부터 올라온 벡터와 자신의 벡터를 순회 돌면서 충돌검사를 시행
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecCollPartInfo.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//자기 자신(동일한 객체)끼리는 충돌 검사를 진행하지 않음.
			if (_ChildvecpCollider[C].pCol == m_vecCollPartInfo[P].pCol)
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
