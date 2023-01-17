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








	////��Ʈ ����� ��� CheckFit���� false�� ��ȯ�Ǿ ���� ����.(��� ���̻� �˻����� ����)
	//if (false == CheckFit(_Partition))
	//{
	//	if (true == IsRootNode())
	//	{
	//		//�� ��쿡�� ���̻� ������ ���� ������ ����.
	//		_Partition.bFinal = true;
	//		m_vecCollPartInfo.push_back(_Partition);
	//		return true;
	//	}

	//	return false;
	//}

	////���� ������ �����ְų�, �ִ� ��� ������ ������ ��尡 �ƴ� ��쿡�� ���̻� �˻����� �ʰ� �����Ѵ�.
	//size_t vecsize = m_vecCollPartInfo.size();
	//if (vecsize < m_pOwner->GetCapacity() || m_iRecursiveLevel >= m_pOwner->GetMaxRecursiveLevel())
	//{
	//	m_vecCollPartInfo.push_back(_Partition);
	//	return true;
	//}
	//else
	//{
	//	//�׷��� ���� ��� �ڽ� ��带 ����� �������� ������ �ڽ����� ����������.
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

	//�������ִ� �����͵��� ��ȸ���鼭 �ڽ� �����̳ʿ� ������ Ȯ��
	size_t size = m_vecCollPartInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		//�̹� Ȯ���� �浹ü�� ��� ��ŵ
		if (true == m_vecCollPartInfo[i].bFinal)
			continue;

		for (int j = 0; j < eQuadrant_End; ++j)
		{
			//��� �ڽ� ��忡�� ���� ������ ��� bFinal�� �Ѽ� ���������� ��带 Ȯ��
			if (false == Insert(m_vecCollPartInfo[i]))
				m_vecCollPartInfo[i].bFinal = true;
		}

	}

	//�ڽ� ��忡 ���������� �� ������ ����
	m_vecCollPartInfo.erase(
		std::remove_if(m_vecCollPartInfo.begin(), m_vecCollPartInfo.end(),
		std::bind(&CQuadTreeNode::CheckNotFinal, this, std::placeholders::_1)), 
		m_vecCollPartInfo.end());
}



bool CQuadTreeNode::CheckFit(const tColliderPartInfo& _Partition)
{
	//AABB�˻縦 ��� ���� �� return
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

	//�浹 ���� ��ü�� ���� ��� ����, �ƴ� ��� �ʷ�

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
	//�ڽ� ���κ��� �ö�� ���Ϳ� �ڽ��� ���͸� ��ȸ ���鼭 �浹�˻縦 ����
	size_t sizeC = _ChildvecpCollider.size();
	for (size_t C = 0; C < sizeC; ++C)
	{
		size_t sizeP = m_vecCollPartInfo.size();
		for (size_t P = 0; P < sizeP; ++P)
		{
			//���� �ּ�(==���� ��ü)�� ��� �˻����� �ʴ´�.
			if (&(_ChildvecpCollider[C]) == &(m_vecCollPartInfo[P]))
				continue;
			Collision(_ChildvecpCollider[C].pCol, m_vecCollPartInfo[P].pCol);
		}
	}

	//���̻� �θ� ��尡 ���� ��� return
	if (nullptr == m_pParent)
		return;

	//�� Ÿ�� �ö󰡸鼭 �ٸ� �θ��� ����Ʈ�� �浹 ��
	m_pParent->CheckCollisionParent(_ChildvecpCollider);
}
