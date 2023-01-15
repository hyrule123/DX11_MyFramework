#include "pch.h"
#include "CSpatialPartition.h"

#include "CTimeMgr.h"

#include "CCollisionMgr.h"

#include "CCollider.h"

CSpatialPartition::CSpatialPartition(eDIMENSION_TYPE _type)
    : m_eDimensionType(_type)
    , m_fGridSize(500.f)
    , m_fGridLBFPos()
    , m_uiGridLevel()
    , m_uiGridSideNum()
    , m_uiGridSideSquare()
    , m_uiGridTotalNum()
    , m_bDynamicSizeOn(true)
    , m_uiMaxGridIndex()
    , m_bTimeRewind()
    , m_fLevelDownTimeRemain(5.f)
    , m_uiGridLevelChangeReserve()
{
}

CSpatialPartition::~CSpatialPartition()
{
}


void CSpatialPartition::ComputeGridArea(CCollider* _pCol, const Vec3& _vColLeftBottomFront, const Vec3& _vColSize)
{
    //Size�� ������ ������� ��.
	assert((_vColSize > 0.f) && (nullptr != _pCol));

    Vec3 vLBF(-m_fGridLBFPos);
    vLBF += _vColLeftBottomFront;

    Vec3 vRTB = vLBF + _vColSize;

    //���ϴ�, ���� ���� x, y, z ��ǥ�� �׸��� �ε��� ��ȣ�� ���
    int GridLevelXYZ_LBF[3] = {};
    int GridLevelXYZ_RTB[3] = {};


    for (UINT i = 0u; i < m_eDimensionType; ++i)
    {
        //���ϴ��� ��� ������ ����, ������ ��� ������ �ø�
        GridLevelXYZ_LBF[i] = (int)(floorf(vLBF[i] / m_fGridSize));
        GridLevelXYZ_RTB[i] = (int)(ceilf(vRTB[i] / m_fGridSize));

        //������ ��� ��� ���� ������ ���� �ְ� �ڸ� Ȯ���� �����س��´�.
        //�̹� �����ӱ����� ������ ���� �ȿ��� ������ �ؾ� ������ �߻����� ����.
        //�̹� ������ ������ ������ ���� �׸��� Ȯ�� �۾��� �����Ѵ�.
        if (GridLevelXYZ_LBF[i] < 0)
        {
            if(true == m_bDynamicSizeOn)
                ReserveLevelUp(Abs(GridLevelXYZ_LBF[i]));

            GridLevelXYZ_LBF[i] = 0;

            m_bTimeRewind = false;
        }

        if (GridLevelXYZ_RTB[i] >= (int)m_uiGridSideNum)
        {
            //m_uiGridSideNum�� ���� ������ ���(NDC ��ǥ��)�̱� ������ abs�� ���� �ʾƵ� ��.
            if (true == m_bDynamicSizeOn)
            {
                ReserveLevelUp(GridLevelXYZ_RTB[i] - (int)m_uiGridSideNum);
            }

            GridLevelXYZ_RTB[i] = (int)m_uiGridSideNum - 1;

            m_bTimeRewind = false;
        }

        //m_bTimeRewind�� true�� ���� �˻�
        if (true == m_bDynamicSizeOn && true == m_bTimeRewind)
        {
            //���� �ε����� �� ������Ʈ�� �ϳ��� ���� ��� �ð��� ���� ������� ����.
            if (0 == GridLevelXYZ_LBF[i] || (m_uiGridSideNum - 1) == GridLevelXYZ_RTB[i])
                m_bTimeRewind = false;
        }
        
    }

    for (int x = GridLevelXYZ_LBF[0]; x <= GridLevelXYZ_RTB[0]; ++x)
    {
        for (int y = GridLevelXYZ_LBF[1]; y <= GridLevelXYZ_RTB[1]; ++y)
        {
            for (int z = GridLevelXYZ_LBF[2]; z <= GridLevelXYZ_RTB[2]; ++z)
            {
                m_vecGrid[m_uiGridSideNum * x + m_uiGridSideNum * y + m_uiGridSideSquare * z].AddCollider(_pCol);
            }
        }
    }
}



void CSpatialPartition::ChangeGridLevel(int _iLevelChange)
{
    int GridLevel = m_uiGridLevel + _iLevelChange;

    if (GridLevel < 0)
        GridLevel = 0;

    m_uiGridLevel = GridLevel;

    //�׸����� �� ���ϴ��� ��ġ�� ���. ������ü �����̹Ƿ� float �ϳ��� �����.
    m_fGridLBFPos = (-1.f) * m_uiGridLevel * m_fGridSize;

    //�� �ܰ谡 �ö󰥼��� ���� ������ 2���� ����
    m_uiGridSideNum = 2u * (m_uiGridLevel + 1u);

    //���� z�� ����� ���� ���� ���� �̸� ���
    m_uiGridSideSquare = m_uiGridSideNum * m_uiGridSideNum;

    //������ ���� ����
    m_uiGridTotalNum = m_uiGridSideNum;
    for (UINT i = 0u; i < m_eDimensionType - 1u; ++i)
        m_uiGridTotalNum *= m_uiGridSideNum;

    //������ �ö��� ��쿡�� resize
    if(_iLevelChange > 0)
        m_vecGrid.resize(m_uiGridTotalNum);
}


void CSpatialPartition::init()
{
	
}

void CSpatialPartition::tick()
{
    if (false == m_bDynamicSizeOn)
        return;

    if (true == m_bTimeRewind)
        m_fLevelDownTimeRemain = 5.f;
    else
    {
        m_fLevelDownTimeRemain -= DELTA_TIME;
        if (m_fLevelDownTimeRemain < 0.f)
        {
            m_fLevelDownTimeRemain = 5.f;
            m_uiGridLevelChangeReserve = -1;
        }
    }

    if (0 == m_uiGridLevel)
        m_bTimeRewind = false;
    else
        m_bTimeRewind = true;

    if (0 != m_uiGridLevelChangeReserve)
    {
        ChangeGridLevel(m_uiGridLevelChangeReserve);
        m_uiGridLevelChangeReserve = 0u;
        m_fLevelDownTimeRemain = 5.f;
    }

    //�ٿ����¡�Ǿ� ����� �پ��ٸ� �ϳ��� pop ���ش�.
    if (m_uiGridTotalNum < m_vecGrid.size())
        m_vecGrid.pop_back();
}

void CSpatialPartition::CheckCollision()
{
    //�׸��� ��ȸ
    for (UINT GridIdx = 0; GridIdx < m_uiGridTotalNum; ++GridIdx)
    {
        const auto& vecCol = m_vecGrid[GridIdx].GetVecCollider();

        //�׸��� ������ �浹ü ���� ��ȸ
        size_t size = vecCol.size();
        for (size_t i = 0; i < size; ++i)
        {
            //�浹ü���� ��ȸ�� ���� �ָ鼭
            for (size_t j = i + 1; j < size; ++j)
            {
                //�켱 ��ȣ�ۿ� �ϴ� ���̾����� Ȯ��
                UINT Layer1 = (UINT)vecCol[i]->GetOwner()->GetLayer();
                UINT Layer2 = (UINT)vecCol[j]->GetOwner()->GetLayer();
                if (true == GetLayerInteract(Layer1, Layer2))
                {
                    CollisionID ID = {};
                    ID.LowID = vecCol[i]->GetID();
                    ID.HighID = vecCol[j]->GetID();

                    //���̵� ���� �� - ���� ������ ����
                    Sort(ID.LowID, ID.HighID);

                    //��ȣ�ۿ� �ϵ��� �����Ǿ����� ��� �浹�˻縦 �����ߴ��� Ȯ��
                    //���� map�� ��ϵǾ� ���� ��� �浹�˻縦 �̹� �����ߴٴ� ���̹Ƿ� ��ŵ�Ѵ�.
                    if (m_umapColData.end() != m_umapColData.find(ID.FullID))
                        continue;

                    //�浹 �˻� �޼ҵ� ȣ��. �� �� �ϳ��� �浹�˻� �޼ҵ常 ȣ���ϸ� ��
                    //���⿡ ���Դٴ� ���� ���� �浹�˻縦 �����Ѵٴ� ����. map�� �浹���θ� make_pair �ؼ� �־���
                    if (vecCol[i]->CheckCollision(vecCol[j]))
                    {
                        m_umapColData.insert(make_pair(ID.FullID, true));
                    } 
                    else
                        m_umapColData.insert(make_pair(ID.FullID, false));
                }


                

                //�浹 �˻�
                
            }
        }
    }
}

