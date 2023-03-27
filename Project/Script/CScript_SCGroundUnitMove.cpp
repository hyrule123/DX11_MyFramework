#include "pch.h"
#include "CScript_SCGroundUnitMove.h"

#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>

CScript_SCGroundUnitMove::CScript_SCGroundUnitMove()
	: CScript(TYPE_INDEX(CScript_SCGroundUnitMove))
	, m_fSpeed(100.f)
	, m_fTurnSpeed(1.f)
	, m_bArrived(true)
{
}

CScript_SCGroundUnitMove::~CScript_SCGroundUnitMove()
{
}


void CScript_SCGroundUnitMove::tick()
{
	if (false == m_bArrived)
	{
		CTransform* pTransform = GetOwner()->Transform();
		Vec2 pos = pTransform->GetRelativePos().XY();

		//이동하기 전 거리 차이 계산
		const Vec2& PrevDist = m_v2Destination - pos;
		
		Vec2 Move = PrevDist;
		Move.Normalize();

		//노멀라이즈 된 값을 활용해서 현재 각도를 구해준다.
		float acos = atan2f(Move.x, Move.y);	
		//if(Move.y < 0.f)

		float CurAngle = Transform()->GetRelativeRot().z;

		pTransform->SetRelativeRotZ(acos);

		

		Move *= m_fSpeed * DELTA_TIME;

		//이동 후 거리 계산
		const Vec2& AfterPos = pos + Move;
		pTransform->SetRelativePosXY(AfterPos);

		//이동 후 위치와 이동 전 위치를 곱한다. 만약 음수가 나올 경우 목적지를 지나친 것이다.
		const Vec2& ArriveCheck = (m_v2Destination - AfterPos) * PrevDist;

		//둘중 하나라도 음수일 경우 지나친 것 - 완료 여부를 true로 변환
		if (ArriveCheck.x < 0.f || ArriveCheck.y < 0.f)
		{
			m_bArrived = true;
		}
	}
}
