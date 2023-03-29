#include "pch.h"
#include "CScript_SCGroundUnitMove.h"

#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CAnimator2D.h>

#include "CScript_SCGroundUnitBase.h"

#include <Engine/CKeyMgr.h>

CScript_SCGroundUnitMove::CScript_SCGroundUnitMove()
	: CFSM(TYPE_INDEX(CScript_SCGroundUnitMove), (UINT)SCGroundUnit::eSTATE::MOVE)
	, m_fSpeed(200.f)
	, m_fTurnSpeed(1.f)
	, m_bArrived(true)
{
}

CScript_SCGroundUnitMove::~CScript_SCGroundUnitMove()
{
}

void CScript_SCGroundUnitMove::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace SCGroundUnit;
		pAnimator->Play(string(strKeyAnim2D[(UINT)eSTATE::MOVE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CScript_SCGroundUnitMove::OnState()
{
	if (false == m_bArrived)
	{
		CTransform* pTransform = GetOwner()->Transform();
		Vec2 pos = pTransform->GetRelativePos().XY();

		//이동하기 전 거리 차이를 계산한다.
		const Vec2& PrevDist = m_v2Destination - pos;

		//방향벡터를 구한다.
		Vec2 Move = PrevDist;
		Move.Normalize();

		//노멀라이즈 된 값을 활용해서 현재 각도를 구해준다.
		float acos = atan2f(Move.y, Move.x);

		//회전 방향은 일치하지만 atan2f는 X축을 기준으로 각도를 구해주고, 
		//Z축은 0도 회전이 위를 보고 있는 상태이므로
		//90도를 더해준다.
		acos -= XM_PIDIV2;

#ifdef _DEBUG

		static float check = 0.f;
		check += DELTA_TIME;
		if (1.f < check)
		{
			string msg = "x: ";
			msg += std::to_string(Move.x);
			msg += ", y: ";
			msg += std::to_string(Move.y);
			msg += ", acos: ";
			msg += std::to_string(acos);
			msg += "\n";
			OutputDebugStringA(msg.c_str());

			check = 0.f;
		}
#endif

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
			GetFSMMgr()->Transition((UINT)SCGroundUnit::eSTATE::IDLE);
		}
	}
}

void CScript_SCGroundUnitMove::EndState()
{
}


void CScript_SCGroundUnitMove::init()
{
	CScript_SCGroundUnitBase* pMgr = static_cast<CScript_SCGroundUnitBase*>(GetHolder()->GetScript(TYPE_INDEX(CScript_SCGroundUnitBase)));

	if (nullptr == pMgr)
	{
		pMgr = new CScript_SCGroundUnitBase;
		GetHolder()->AddScript(pMgr);
	}

	pMgr->AddFSM(GetMyState(), this);
}

void CScript_SCGroundUnitMove::tick()
{
	assert(GetFSMMgr());

	CScript_SCGroundUnitBase* pMgr = static_cast<CScript_SCGroundUnitBase*>(GetFSMMgr());

	//자신의 유닛이 선택되어 있고, 우클릭(이동명령)이 들어왔을 경우 상태 전환을 요청
	if (pMgr->IsSelected() && KEY_TAP(eKEY::RBTN))
	{
		pMgr->Transition(GetMyState());
	}
}
