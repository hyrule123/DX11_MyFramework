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
	//�켱 �ڽ��� ��� �ȿ� ���� �� �ִ����� Ȯ��
	if (false == CheckFit(_Partition))
	{
		//��Ʈ ����� ��� �ڽź��� ũ���� �ϴ� ����
		if (true == IsRootNode())
		{
			m_vecCollPartInfo.push_back(_Partition);
			m_vecCollPartInfo.back().bFinal = true;
			return true;
		}

		return false;
	}

	//���� �ڽ��� ��忡 ���� �� ���� ��� 
	if (nullptr == m_arrChild[0])
	{
		//���� �ڸ��� ���� �ְų� ��� �ܰ谡 �� �ܰ迡 ���������� �ٷ� ����
		if (m_vecCollPartInfo.size() <= m_pOwner->GetCapacity() 
			|| 
			m_iRecursiveLevel >= m_pOwner->GetMaxRecursiveLevel())
		{
			m_vecCollPartInfo.push_back(_Partition);
			return true;
		}
		//�׷��� ���� ��� �ڽ� ������Ʈ�� �����ϰ� ���� �߰������� �� ����
		else
		{ 
			Split();
		}
	}

	//Split�� ����� �� �������� �Ѿ���� ��Ƽ���� �ڽĳ�忡 ���� �� �ִ��� Ȯ���Ѵ�.
	for (int i = 0; i < eQuadrant_End; ++i)
	{
		if (true == m_arrChild[i]->Insert(_Partition))
			return true;
	}

	//�ڽĳ�� �ʿ� �߰��� �õ��غôµ� ��� �ڽĳ�鿡 ���� ���� �ʾ�����
	//���������� �ڽ��� �����̳ʿ� �����ϰ� true�� ��ȯ�Ѵ�.
	m_vecCollPartInfo.push_back(_Partition);
	m_vecCollPartInfo.back().bFinal = true;
	return true;
}



void CQuadNode::Split()
{
	float fSizeHalf = m_SquareInfo.Size * 0.5f;
	int iLevel = m_iRecursiveLevel + 1;

	//�ڽ��� ���� �������� �����ؼ� ���ο� ��带 �߰��Ѵ�.
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


	//�������ִ� �����͵��� ��ȸ���鼭 �ڽ� �����̳ʿ� ������ Ȯ��
	size_t size = m_vecCollPartInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		//�̹� Ȯ���� �浹ü�� ��� ��ŵ
		if (true == m_vecCollPartInfo[i].bFinal)
			continue;

		for (int j = 0; j < eQuadrant_End; ++j)
		{
			//�ڽ� ��忡 ���� ��� ���� �浹ü�� ����
			if (true == m_arrChild[j]->Insert(m_vecCollPartInfo[i]))
				break;
		}

		//for������ break���� ���ϰ� ������� �� == ����� �´� �ڽ� ��尡 ����->final on
		m_vecCollPartInfo[i].bFinal = true;
	}

	//�ڽ� ��忡 ���������� �� ������ ����
	m_vecCollPartInfo.erase(
		std::remove_if(m_vecCollPartInfo.begin(), m_vecCollPartInfo.end(),
		std::bind(&CQuadNode::CheckNotFinal, this, std::placeholders::_1)), 
		m_vecCollPartInfo.end());
}







void CQuadNode::Collision(CCollider2D* _pColA, CCollider2D* _pColB)
{
	//�� ���̾ �켱 ��ȣ�ۿ��ϴ������� �˻�. ��ȣ�ۿ� �� �� ��� continue
	if (false == m_pOwner->CheckLayerInteract(
		_pColA->GetLayerIndex(),
		_pColB->GetLayerIndex()))
		return;

	//ID�� �����
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

	//���� �����ӿ� �浹�߾������� Ȯ��
	bool PrevColl = m_pOwner->GetPrevCollided(ColID);

	//�浹�� Ȯ�εǾ��� ���
	if (_pColA->CheckCollision(_pColB))
	{
		//ù �浹�� ��� BeginCollision
		if (true != PrevColl)
		{
			_pColA->BeginCollision(_pColB);

			//�浹���� �浹ü ���� �ʿ� �浹������ ���
			tCollisionInfo Info = {};
			Info.pColliderA = _pColA;
			Info.pCOlliderB = _pColB;
			Info.bCurrent = true;
			m_pOwner->AddCollisionInfo(ColID, Info);
		}
		//���� �����ӿ��� �浹 ���̾��� ���
		else
		{
			//OnCollision�� ȣ��
			_pColA->OnCollision(_pColB);

			//�浹���� �浹ü ���� �ʿ��� �浹������ ����
			m_pOwner->SetCollisionInfoChecked(ColID);
		}
	}
	//�浹���� �ʾ��� ��� + ���� �����ӿ� �浹���̾��� ��� = �浹 ����
	else if(true == PrevColl)
	{
		_pColA->EndCollision(_pColB);

		//�浹���� �浹ü ���� �ʿ��� �浹������ ����
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
	//�ڽ��� �� ���� ���� = ũ��
	//float RootSideLen = m_pOwner->GetSquareSize();
	float mySideLen = m_SquareInfo.Size;
	Vec3 vSize(mySideLen, mySideLen, 1.f);
	const Matrix& matScale = Matrix::CreateScale(vSize);
	
	//�ڽ��� ������
	float SizeHalf = m_SquareInfo.Size * 0.5f;
	Vec3 vPos(m_SquareInfo.LB_X + SizeHalf, m_SquareInfo.LB_Y + SizeHalf, 0.f);

	//�������� ��Ʈ ����� �� ���� ���̿� ���ؼ� ��ġ�� �˾Ƴ���.
	//vPos *= RootSideLen;
	const Matrix& matTrans = Matrix::CreateTranslation(vPos);


	tDebugShapeInfo Info = {};
	Info.eShape = eSHAPE_RECT;
	Info.matWorld = matScale * matTrans;

	//�浹 ���� ��ü�� ���� ��� ����, �ƴ� ��� �ʷ�
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
	//�ڽ� ��尡 ���� ��� �ڽĳ���� CheckCollision�� ���� ȣ��
	if (nullptr != m_arrChild[0])
	{
		//�ڽ� ���鵵 CheckCollidion ȣ��
		for (int i = 0; i < eQuadrant_End; ++i)
		{
			m_arrChild[i]->CheckCollision();
		}
	}

	//�ڽ��� CheckCollisionParent�� ȣ��. �ڽź��� �����ؼ� ��Ʈ ������ Ÿ�� �ö󰡸鼭 ��� �浹ü�� �浹�� �˻�.
	CheckCollisionParent(m_vecCollPartInfo);
}

void CQuadNode::CheckCollisionParent(vector<tColliderPartInfo>& _ChildvecpCollider)
{
	//�ڽ� ���κ��� �ö�� ���Ϳ� �ڽ��� ���͸� ��ȸ ���鼭 �浹�˻縦 ����
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecCollPartInfo.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//�ڱ� �ڽ�(������ ��ü)������ �浹 �˻縦 �������� ����.
			if (_ChildvecpCollider[C].pCol == m_vecCollPartInfo[P].pCol)
				continue;

			Collision(_ChildvecpCollider[C].pCol, m_vecCollPartInfo[P].pCol);
		}
	}

	//���̻� �θ� ��尡 ���� ��� ���� ������ ���̹Ƿ� �ش� �浹ü ���͸� ���� return
	if (nullptr == m_pParent)
	{
		_ChildvecpCollider.clear();
		return;
	}

	//�� Ÿ�� �ö󰡸鼭 �ٸ� �θ��� ����Ʈ�� �浹 ��
	m_pParent->CheckCollisionParent(_ChildvecpCollider);
}
