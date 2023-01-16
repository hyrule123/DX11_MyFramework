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
		//�ϴ� �����͸� �߰��Ѵ�.
		m_vecpCollider.push_back(_Partition);

		//Capacity �̻����� �����Ͱ� ���ְ�, �ִ� ��� ���ر��� �������� �ʾҴٸ� ���� ��带 �����ؼ� �����͸� ����������.
		if ((m_vecpCollider.size() > m_pOwner->GetCapacity()) && m_pOwner->GetMaxRecursiveLevel() > m_iRecursiveLevel)
		{
			//���� ��� ���� �� ���� ���� �����͸� ���� ������ �޼ҵ�
			Split();
		}
	}

	//���� �ڽ� ��尡 ���� ��� �����ϰ� �Ǵ� �ڵ��̴�.
	//�ڽ� ������ ��ȸ���鼭 �߰������� �ٿ����� �� �ִ��� Ȯ���Ѵ�.
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

		//���ǿ� �´� �ڽ� ��尡 ���� ��� �ű⿡ ��������� �����Ѵ�.
		m_arrChild[i]->Insert(_Partition);
		return;
	}
	
	//��� �ڽ� ��忡 ���� �ʾ��� ��� ���⿡ ����.
	_Partition.bFinal = true;
	m_vecpCollider.push_back(_Partition);
}

void CQuadTreeNode::Split()
{
	float fSizeHalf = m_SquareInfo.Size * 0.5f;
	int iLevel = m_iRecursiveLevel + 1;

	//�ڽ��� ���� �������� �����ؼ� ���ο� ��带 �߰��Ѵ�.
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
	//�浹���� �ʾ��� ��� + ���� �����ӿ� �浹���̾��� ���
	else if(true == PrevColl)
	{
		_pColA->EndCollision(_pColB);

		//�浹���� �浹ü ���� �ʿ��� �浹������ ����
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

	//�浹 ���� ��ü�� ���� ��� ����, �ƴ� ��� �ʷ�
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
	//�ڽ� ���κ��� �ö�� ���Ϳ� �ڽ��� ���͸� ��ȸ ���鼭 �浹�˻縦 ����
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecpCollider.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//���� �ּ�(==���� ��ü)�� ��� �˻����� �ʴ´�.
			if (&(_ChildvecpCollider[C]) == &(m_vecpCollider[P]))
				continue;
			Collision(_ChildvecpCollider[C].pCol, m_vecpCollider[P].pCol);
		}
	}

	//���̻� �θ� ��尡 ���� ��� return
	if (nullptr == m_pParent)
		return;

	//�� Ÿ�� �ö󰡸鼭 �ٸ� �θ��� ����Ʈ�� �浹 ��
	m_pParent->CheckCollisionParent(_ChildvecpCollider);
}
