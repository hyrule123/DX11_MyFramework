#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CCollider2D.h"

#include "CTimeMgr.h"


#include "CCollider2D_OBB.h"
#include "CCollider2D_Point.h"

CCollisionMgr::CCollisionMgr()
	: m_iNum2DGridX()
	, m_iNum2DGridY()
	, m_iNum2DGridTotalIndex()
	, m_arrFlagLayerInteraction{}
{
	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)10, (UINT)10);
}

CCollisionMgr::~CCollisionMgr()
{

}


void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, const vector<Vec2>& _vecPoint)
{
	size_t size = _vecPoint.size();
	vector<UINT> vecIdx;

	for (size_t i = 0; i < size; ++i)
	{
		//�׸��� ������ ��ǥ��� ��ȯ�Ѵ�.
		Vec2 Point = _vecPoint[i] - m_v2DSpaceLB;

		//�� �� ĭ�� �ε����� ����.
		Point *= m_v2DGridSizeInv;

		//�ε����� ������ �׸��� ���� ������ ����
		int IdxX = std::clamp((int)Point.x, 0, m_iNum2DGridX);
		int IdxY = std::clamp((int)Point.y, 0, m_iNum2DGridY);
		UINT Idx = (UINT)(IdxX * IdxY);

		m_vec2DGrid[Idx].vecColl.push_back(_pCol);

		vecIdx.push_back(Idx);
	}

	//���� �ε��� ������ �Ѱ��ش�.
	//Ʈ������ ������ ����Ǳ� �������� ���̻� ���� ���� ���� �浹ü ������ ����� �ʿ䰡 ����.
	_pCol->SetvecGridIdx(vecIdx);
}

void CCollisionMgr::AddCollider2D(CCollider2D* _pCol, const vector<UINT>& _vecIdx)
{
	//�׳� ��ȸ �����ָ鼭 �׸��忡 �浹�˻��ؾߵ� �浹ü ������ ����ִ´�.
	size_t size = _vecIdx.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vec2DGrid[_vecIdx[i]].vecColl.push_back(_pCol);
	}
}



void CCollisionMgr::Create2DGrid(Vec2 _vWorldLB, Vec2 _vWorldSize, UINT _uiGridNumX, UINT _uiGridNumY)
{
	m_iNum2DGridX = _uiGridNumX;
	m_iNum2DGridY = _uiGridNumY;

	m_v2DSpaceLB = _vWorldLB;
	
	m_v2DSpaceSize = _vWorldSize;

	float GridSizeX = _vWorldSize.x / _uiGridNumX;
	float GridSizeY = _vWorldSize.y / _uiGridNumY;
	m_v2DGridSize = Vec2(GridSizeX, GridSizeY);


	//�������� �������� �����Ƿ�, �Ի� ����ȭ�� ���� ������ ���� �̸� ���� ���´�.
	m_v2DGridSizeInv.x = 1.f / GridSizeX;
	m_v2DGridSizeInv.y = 1.f / GridSizeY;


	m_iNum2DGridTotalIndex = m_iNum2DGridX * m_iNum2DGridY;
	m_vec2DGrid.resize(m_iNum2DGridTotalIndex);
}

void CCollisionMgr::tick()
{
	for (int i = 0; i < m_iNum2DGridTotalIndex; ++i)
	{
		size_t size = m_vec2DGrid[i].vecColl.size();
		//�浹�� �ּ� 2���� �־�� ���� �����ϹǷ� 2����� 2�� ������ ��� ����ְ� ���� ���Ϳ� ���� �浹�˻� ����
		if (size < 2)
		{
			m_vec2DGrid[i].vecColl.clear();
			continue;
		}
			
		//�׸��� ���θ� ID ������� �����Ѵ�.
		std::sort(m_vec2DGrid[i].vecColl.begin(), m_vec2DGrid[i].vecColl.end(),
			[](CCollider2D* _pColA, CCollider2D* _pColB)->bool
			{
				return _pColA->GetID() < _pColB->GetID();
			}
		);

		for (int l = 0; l < size; ++l)
		{
			for (int m = l + 1; m < size; ++m)
			{
				//�� ���̾ �浹�ϵ��� �����Ǿ� �ִ��� ���θ� �˻��Ѵ�. �浹���� �ʴ� ���̾��� ��� �浹 �˻� X
				if (false == (m_arrFlagLayerInteraction[m_vec2DGrid[i].vecColl[l]->GetLayerIndex()]
					&
					(UINT32)1 << m_vec2DGrid[i].vecColl[m]->GetLayerIndex())
					)
					continue;

				//������ ID�� ������������ ���������Ƿ� ID�� ���� ���� �ʿ䰡 ����.
				CollisionID ID = {}; 
				ID.LowID = m_vec2DGrid[i].vecColl[l]->GetID();
				ID.HighID = m_vec2DGrid[i].vecColl[m]->GetID();

				CCollider2D* pColA = m_vec2DGrid[i].vecColl[l];
				CCollider2D* pColB = m_vec2DGrid[i].vecColl[m];

				//�浹
				if (true == CheckCollision2D(pColA, pColB))
				{

					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter == m_umapCollisionID.end())
					{
						pColA->BeginCollision(pColB);


						//�浹 ������ �浹�� üũ�� �ð��� �浹ü �ּҸ� �־ map�� �����Ѵ�.
						tCollisionInfo Info = 
						{ CTimeMgr::GetInst()->GetCurCount(), 
							pColA, pColB };
						m_umapCollisionID.insert(make_pair(ID.FullID, Info));
					}
					else
					{
						pColA->OnCollision(pColB);

						//�浹 ���̶��(map�� �浹 ������ ��� �ִٸ�) üũ�� �ð��� �����Ѵ�.
						iter->second.llCheckedCount = CTimeMgr::GetInst()->GetCurCount();
					}

				}
				//���浹
				else
				{
					auto iter = m_umapCollisionID.find(ID.FullID);
					if (iter != m_umapCollisionID.end())
					{
						//�浹�� �����µ� ID�� �����ִٸ� �浹 �� �Լ��� ȣ���ϰ� �浹 ������ �����Ѵ�.
						pColA->EndCollision(pColB);

						m_umapCollisionID.erase(iter);
					}

				}
			}
		}

		//�׸��� ������ �浹ü ������ ����ش�.
		m_vec2DGrid[i].vecColl.clear();
	}

	//�̹� Ÿ�ӿ� �浹Ȯ�ε��� ���� �浹ü�鿡 ���ؼ� �浹 ���� ó��
	//�̹� ƽ�� ��ϵ� �������� �޾ƿ´�.
	LONGLONG CurTime = CTimeMgr::GetInst()->GetCurCount();
	auto iter = m_umapCollisionID.begin();
	const auto& iterEnd = m_umapCollisionID.end();
	while (iter != iterEnd)
	{
		//��ϵ� �ð��� �̹� tick�� �ð��� �ƴ� ��� �浹 ��ü ó���ϰ� �����Ѵ�.
		if (CurTime != iter->second.llCheckedCount)
		{
			iter->second.pColliderA->EndCollision(iter->second.pColliderB);
			iter = m_umapCollisionID.erase(iter);
		}
		else
			++iter;
	}

}

bool CCollisionMgr::CheckCollision2D(CCollider2D* _pCol_1, CCollider2D* _pCol_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Rect(CCollider2D_Rect* _pColRect_1, CCollider2D_Rect* _pColRect_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Circle(CCollider2D_Circle* _pColCircle_1, CCollider2D_Circle* _pColCircle_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_OBB2D_OBB2D(CCollider2D_OBB* _pColOBB2D_1, CCollider2D_OBB* _pColOBB2D_2)
{
	const tOBB2D& OBB_1 = _pColOBB2D_1->GetColliderInfo();
	const tOBB2D& OBB_2 = _pColOBB2D_2->GetColliderInfo();

	//Vec2�� ������ * 2�� ����Ѵ�. ����ȭ�� ���� ó�� ����� ���� ��� ���.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//�� ���� ������ ��ȸ�ϱ� ���ϵ��� �����´�.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &OBB_1, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &OBB_2, vec2_2size);


	Vec2 VecMiddle = OBB_1.m_vMiddle - OBB_2.m_vMiddle;
	for (int i = 0; i < 4; ++i)
	{
		Vec2 norm = arrVec[i];
		norm.Normalize();

		float fProjSum = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjSum += fabsf(norm.Dot(arrVec[j]));
		}

		//�� ���� ����(vSize)�� ���������Ƿ� 0.5�� ������� ���� ���� ���̰� ��
		fProjSum *= 0.5f;

		float fCenterProj = fabsf(norm.Dot(VecMiddle));

		if (fProjSum < fCenterProj)
			return false;
	}
	

	//�� �˻縦 ����ϸ� HitPoint�� ����ؼ� �� �浹ü�� �浹�����Լ��� ȣ���Ѵ�.



	return true;
}

bool CCollisionMgr::CheckCollision2D_OBB2D_Point(CCollider2D_OBB* _pColOBB2D, CCollider2D_Point* _pColPoint)
{
	return false;
}
