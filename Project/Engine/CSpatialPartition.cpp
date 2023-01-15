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
    //Size는 무조건 양수여야 함.
	assert((_vColSize > 0.f) && (nullptr != _pCol));

    Vec3 vLBF(-m_fGridLBFPos);
    vLBF += _vColLeftBottomFront;

    Vec3 vRTB = vLBF + _vColSize;

    //좌하단, 우상단 각각 x, y, z 좌표의 그리드 인덱스 번호를 계산
    int GridLevelXYZ_LBF[3] = {};
    int GridLevelXYZ_RTB[3] = {};


    for (UINT i = 0u; i < m_eDimensionType; ++i)
    {
        //우하단의 경우 무조건 내림, 우상단의 경우 무조건 올림
        GridLevelXYZ_LBF[i] = (int)(floorf(vLBF[i] / m_fGridSize));
        GridLevelXYZ_RTB[i] = (int)(ceilf(vRTB[i] / m_fGridSize));

        //범위를 벗어날 경우 범위 안으로 돌려 주고 자리 확장을 예약해놓는다.
        //이번 프레임까지는 지정된 범위 안에서 연산을 해야 오차가 발생하지 않음.
        //이번 프레임 연산이 끝나고 나면 그리드 확장 작업을 진행한다.
        if (GridLevelXYZ_LBF[i] < 0)
        {
            if(true == m_bDynamicSizeOn)
                ReserveLevelUp(Abs(GridLevelXYZ_LBF[i]));

            GridLevelXYZ_LBF[i] = 0;

            m_bTimeRewind = false;
        }

        if (GridLevelXYZ_RTB[i] >= (int)m_uiGridSideNum)
        {
            //m_uiGridSideNum의 값은 무조건 양수(NDC 좌표계)이기 때문에 abs를 하지 않아도 됨.
            if (true == m_bDynamicSizeOn)
            {
                ReserveLevelUp(GridLevelXYZ_RTB[i] - (int)m_uiGridSideNum);
            }

            GridLevelXYZ_RTB[i] = (int)m_uiGridSideNum - 1;

            m_bTimeRewind = false;
        }

        //m_bTimeRewind가 true일 때만 검사
        if (true == m_bDynamicSizeOn && true == m_bTimeRewind)
        {
            //끝쪽 인덱스에 들어간 오브젝트가 하나라도 있을 경우 시간을 새로 등록하지 않음.
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

    //그리드의 맨 좌하단의 위치를 계산. 정육면체 형태이므로 float 하나면 충분함.
    m_fGridLBFPos = (-1.f) * m_uiGridLevel * m_fGridSize;

    //한 단계가 올라갈수록 변의 갯수는 2개씩 증가
    m_uiGridSideNum = 2u * (m_uiGridLevel + 1u);

    //빠른 z축 계산을 위한 제곱 값도 미리 계산
    m_uiGridSideSquare = m_uiGridSideNum * m_uiGridSideNum;

    //차원에 따라서 제곱
    m_uiGridTotalNum = m_uiGridSideNum;
    for (UINT i = 0u; i < m_eDimensionType - 1u; ++i)
        m_uiGridTotalNum *= m_uiGridSideNum;

    //레벨이 올랐을 경우에는 resize
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

    //다운사이징되어 사이즈가 줄었다면 하나씩 pop 해준다.
    if (m_uiGridTotalNum < m_vecGrid.size())
        m_vecGrid.pop_back();
}

void CSpatialPartition::CheckCollision()
{
    //그리드 순회
    for (UINT GridIdx = 0; GridIdx < m_uiGridTotalNum; ++GridIdx)
    {
        const auto& vecCol = m_vecGrid[GridIdx].GetVecCollider();

        //그리드 내부의 충돌체 벡터 순회
        size_t size = vecCol.size();
        for (size_t i = 0; i < size; ++i)
        {
            //충돌체끼리 순회를 돌아 주면서
            for (size_t j = i + 1; j < size; ++j)
            {
                //우선 상호작용 하는 레이어인지 확인
                UINT Layer1 = (UINT)vecCol[i]->GetOwner()->GetLayer();
                UINT Layer2 = (UINT)vecCol[j]->GetOwner()->GetLayer();
                if (true == GetLayerInteract(Layer1, Layer2))
                {
                    CollisionID ID = {};
                    ID.LowID = vecCol[i]->GetID();
                    ID.HighID = vecCol[j]->GetID();

                    //아이디를 낮은 순 - 높은 순으로 정렬
                    Sort(ID.LowID, ID.HighID);

                    //상호작용 하도록 설정되어있을 경우 충돌검사를 진행했는지 확인
                    //만약 map에 등록되어 있을 경우 충돌검사를 이미 진행했다는 뜻이므로 스킵한다.
                    if (m_umapColData.end() != m_umapColData.find(ID.FullID))
                        continue;

                    //충돌 검사 메소드 호출. 둘 중 하나의 충돌검사 메소드만 호출하면 됨
                    //여기에 들어왔다는 것은 최초 충돌검사를 진행한다는 뜻임. map에 충돌여부를 make_pair 해서 넣어줌
                    if (vecCol[i]->CheckCollision(vecCol[j]))
                    {
                        m_umapColData.insert(make_pair(ID.FullID, true));
                    } 
                    else
                        m_umapColData.insert(make_pair(ID.FullID, false));
                }


                

                //충돌 검사
                
            }
        }
    }
}

