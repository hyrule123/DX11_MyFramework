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
	RegisterCollisionFunc();




	AddLayerInteraction2D(1, 1);
	Create2DGrid(Vec2(-10000.f, -10000.f), Vec2(20000.f, 20000.f), (UINT)10, (UINT)10);
}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::CalcAndAddCollider2D(__in CCollider2D* _pCol, __in Vec4 _vLBRTPos, __out vector<UINT>& _vecIdx)
{
	_vecIdx.clear();

	//�׸��� ������ ��ǥ��� ��ȯ�Ѵ�.
	_vLBRTPos -= m_v4SpaceLBPos;
	//�׸��� �� ĭ�� ������� ���� �ָ� �� �� ĭ�� �ε��� ��ġ�� ����� �� ����.
	_vLBRTPos *= m_v4GridSizeInv;

	//�ε����� ������ �׸��� ���� ������ ����
	enum { BeginX, BeginY, EndX, EndY };
	int iBeginXYEndXY[4] = {};
	iBeginXYEndXY[BeginX] = std::clamp((int)_vLBRTPos.x, 0, m_iNum2DGridX);
	iBeginXYEndXY[BeginY] = std::clamp((int)_vLBRTPos.y, 0, m_iNum2DGridY);
	iBeginXYEndXY[EndX] = std::clamp((int)_vLBRTPos.z, 0, m_iNum2DGridX);
	iBeginXYEndXY[EndY] = std::clamp((int)_vLBRTPos.w, 0, m_iNum2DGridY);

	for (int x = iBeginXYEndXY[BeginX]; x <= iBeginXYEndXY[EndX]; ++x)
	{
		for (int y = iBeginXYEndXY[BeginY]; y <= iBeginXYEndXY[EndY]; ++y)
		{
			UINT Idx = (UINT)(y * m_iNum2DGridX + x);
			
			m_vec2DGrid[Idx].vecColl.push_back(_pCol);

			_vecIdx.push_back(Idx);
		}
	}
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

	m_vSpaceLBPos = _vWorldLB;
	m_v4SpaceLBPos = Vec4(m_vSpaceLBPos.x, m_vSpaceLBPos.y, m_vSpaceLBPos.x, m_vSpaceLBPos.y);
	
	m_vSpaceSize = _vWorldSize;

	float GridSizeX = _vWorldSize.x / _uiGridNumX;
	float GridSizeY = _vWorldSize.y / _uiGridNumY;
	m_vGridSize = Vec2(GridSizeX, GridSizeY);


	//�������� �������� �����Ƿ�, �Ի� ����ȭ�� ���� ������ ���� �̸� ���� ���´�.
	m_vGridSizeInv.x = 1.f / GridSizeX;
	m_vGridSizeInv.y = 1.f / GridSizeY;
	m_v4GridSizeInv = Vec4(m_vGridSizeInv.x, m_vGridSizeInv.y, m_vGridSizeInv.x, m_vGridSizeInv.y);


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
	return m_arrFuncCheckCollision2D[(int)_pCol_1->GetColliderType()][(int)_pCol_2->GetColliderType()](_pCol_1, _pCol_2);
}

bool CCollisionMgr::CheckCollision2D_Rect_Rect(CCollider2D* _pColRect_1, CCollider2D* _pColRect_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Circle(CCollider2D* _pColRect, CCollider2D* _pColCircle)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_OBB(CCollider2D* _pColRect, CCollider2D* _pColOBB)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Rect_Point(CCollider2D* _pColRect, CCollider2D* _pColPoint)
{


	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Circle(CCollider2D* _pColCircle_1, CCollider2D* _pColCircle_2)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_OBB(CCollider2D* _pColCircle, CCollider2D* _pColOBB)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_Circle_Point(CCollider2D* _pColCircle, CCollider2D* _pColPoint)
{
	return false;
}

bool CCollisionMgr::CheckCollision2D_OBB_OBB(CCollider2D* _pColOBB2D_1, CCollider2D* _pColOBB2D_2)
{
	const tOBB2D& OBB_1 = static_cast<CCollider2D_OBB*>(_pColOBB2D_1)->GetColliderInfo();
	const tOBB2D& OBB_2 = static_cast<CCollider2D_OBB*>(_pColOBB2D_2)->GetColliderInfo();

	//Vec2�� ������ * 2�� ����Ѵ�. ����ȭ�� ���� ó�� ����� ���� ��� ���.
	static const size_t vec2_2size = sizeof(Vec2) * 2;

	//�� ���� ������ ��ȸ�ϱ� ���ϵ��� �����´�.
	//OBB ����ü�� ���� ù �κп��� X��, Y�࿡ ���� ������ ��� ����. memcpy�� �������ָ� ��.
	Vec2 arrVec[4] = {};
	memcpy_s(&arrVec[0], vec2_2size, &OBB_1, vec2_2size);
	memcpy_s(&arrVec[2], vec2_2size, &OBB_2, vec2_2size);


	Vec2 VecMiddle = OBB_1.m_vCenterPos - OBB_2.m_vCenterPos;
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

bool CCollisionMgr::CheckCollision2D_OBB_Point(CCollider2D* _pColOBB2D, CCollider2D* _pColPoint)
{
	const tOBB2D& OBBInfo = static_cast<CCollider2D_OBB*>(_pColOBB2D)->GetColliderInfo();
	Vec2 vPointToOBBCenter = _pColPoint->GetCenterPos();

	//���� ��ġ�κ��� OBB�� �߽� ��ġ���� ���ش�.
	vPointToOBBCenter -= OBBInfo.m_vCenterPos;

	for (int i = 0; i < (int)eAXIS2D::END; ++i)
	{
		//�翵�� ����� �� ���� �������ͷ� ��ȯ
		Vec2 Norm = OBBInfo.m_vAxis[i];
		Norm.Normalize();

		//�� ���� �翵 ��� �࿡ �翵�ϰ�, 0.5�� ���ؼ� �� ���̷� ���δ�.(�߽ɺ����� �Ÿ��̹Ƿ�)
		float NormLen = 0.f;
		NormLen += fabs(Norm.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
		NormLen += fabs(Norm.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
		NormLen *= 0.5f;

		//���������� ���� OBB�� �߽����� ���� �࿡ �翵�Ѵ�.
		float PointToOBBCenterProj = fabs(Norm.Dot(vPointToOBBCenter));
		
		//���� XY���� �翵�� ���̰� ���� ������ �Ÿ����� �� ��� �浹�� �ƴϴ�.
		if (PointToOBBCenterProj > NormLen)
			return false;
	}

	////X�� �翵
	//Vec2 NormX = OBBInfo.m_vAxis[(int)eAXIS2D::X];
	//NormX.Normalize();
	//float NormXLen = 0.f;
	//NormXLen += fabs(NormX.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
	//NormXLen += fabs(NormX.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
	//NormXLen *= 0.5f;

	//float PointToOBBCenterProjX = fabs(NormX.Dot(vPointToOBBCenter));

	//if (PointToOBBCenterProjX > NormXLen)
	//	return false;


	//Vec2 NormY = OBBInfo.m_vAxis[(int)eAXIS2D::Y];
	//NormY.Normalize();
	//float NormYLen = 0.f;
	//NormYLen += fabs(NormY.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::X]));
	//NormYLen += fabs(NormY.Dot(OBBInfo.m_vAxis[(int)eAXIS2D::Y]));
	//NormYLen *= 0.5f;

	//float PointToOBBCenterProjY = fabs(NormY.Dot(vPointToOBBCenter));

	//if (PointToOBBCenterProjY > NormYLen)
	//	return false;


	return true;
}


bool CCollisionMgr::CheckCollision2D_Point_Point(CCollider2D* _pColPoint_1, CCollider2D* _pColPoint_2)
{
	return false;
}

void CCollisionMgr::RegisterCollisionFunc()
{
	//�ε����� �Լ��� ������ ����ġ�� ��� std::placeholders�� �������� ��ġ�Ұ�

	//RECTANGLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Rect, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::RECT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, std::placeholders::_1, std::placeholders::_2);

	//CIRCLE
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Circle, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Circle, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::CIRCLE][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, std::placeholders::_1, std::placeholders::_2);

	//OBB
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_OBB, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_OBB, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_OBB, this, std::placeholders::_1, std::placeholders::_2);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::OBB][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, std::placeholders::_1, std::placeholders::_2);


	//POINT
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::RECT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Rect_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::CIRCLE]
		= std::bind(&CCollisionMgr::CheckCollision2D_Circle_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::OBB]
		= std::bind(&CCollisionMgr::CheckCollision2D_OBB_Point, this, std::placeholders::_2, std::placeholders::_1);
	m_arrFuncCheckCollision2D[(int)eCOLLIDER_TYPE_2D::POINT][(int)eCOLLIDER_TYPE_2D::POINT]
		= std::bind(&CCollisionMgr::CheckCollision2D_Point_Point, this, std::placeholders::_1, std::placeholders::_2);
}
