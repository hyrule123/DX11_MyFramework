#include "pch.h"
#include "cScript_MouseCursor.h"

#include <Engine/cKeyMgr.h>
#include <Engine/cRenderMgr.h>
#include <Engine/cCom_Camera.h>
#include <Engine/cTransform.h>
#include <Engine/ICollider.h>

#include <Engine/cGameObject.h>

#include "define_SC.h"
//#include "cScript_FSM_Move_Ground.h"
//#include "cScript_FSM_Attack.h"


#include <Engine/S_H_Struct.hlsli>

cScript_MouseCursor::cScript_MouseCursor()
	: m_arrpFuncLBTNCallback{}
	, m_arrpFuncRBTNCallback{}
	, m_pSelectedGameObject()
	, m_pObj_ZCheck()
{
}

cScript_MouseCursor::~cScript_MouseCursor()
{
}



void cScript_MouseCursor::Tick()
{
	CalcPos();
	
	if (nullptr != m_pObj_ZCheck)
	{
		m_pSelectedGameObject = m_pObj_ZCheck;
		m_pObj_ZCheck = nullptr;
	}	

	if (m_pSelectedGameObject)
	{
		//파괴처리 되었을 경우 선택 해제
		if (m_pSelectedGameObject->IsDestroyed())
			m_pSelectedGameObject = nullptr;

		//아닐 경우 + 우클릭을 했을 경우 이동 크립트를 가져와서 이동시킨다.
		else if(KEY_DOWN(eKEY::RBTN))
		{
			//(cFSM_Move_Base*)(m_pSelectedGameObject->ScriptHolder()->Transition((UINT)SC::FSM::MOVE));
			//cFSM_Move_Base* pMoveState = (cFSM_Move_Base*)m_pSelectedGameObject->ScriptHolder()->GetFSM(SC::FSM::MOVE);
				
			//if (pMoveState)
			//{
			//	pMoveState->SetDestination(m_v2CursorPos);
			//}
		}

		else if (KEY_DOWN(eKEY::A))
		{
			//cScript_FSM_Attack* pAttackState = (cScript_FSM_Attack*)(m_pSelectedGameObject->ScriptHolder()->Transition((UINT)SC::FSM::ATTACK));
		}
	}

	
}

void cScript_MouseCursor::OnCollision(ICollider* _Collider, const Vec3& _v3HitPoint)
{
	//오브젝트가 삭제 대기 상태일 경우 여기에 아예 들어오지 않으므로 걱정할 필요 없음.

	//3D에서는 Ray를 통해서 해야함.
	//충돌중인데 좌클릭을 누른 경우
	if (KEY_DOWN(eKEY::LBTN))
	{	
		if (eDIMENSION_TYPE::_2D == _Collider->GetDimensionType())
		{
			//등록된 오브젝트 없을 경우 일단 등록
			if (nullptr == m_pObj_ZCheck)
				m_pObj_ZCheck = _Collider->GetOwner();
			else
			{
				//새로 들어온 오브젝트의 Z축이 더 앞에 있을 경우 해당 오브젝트를 등록한다.
				float originalZ = m_pObj_ZCheck->Transform().GetWorldPos().z;
				float newZ = _Collider->GetOwner()->Transform().GetWorldPos().z;
				if (newZ < originalZ)
					m_pObj_ZCheck = _Collider->GetOwner();
			}
		}
	}

	cKeyMgr* pKeyMgr = cKeyMgr::GetInst();
	for (int i = 0; i < (int)eKEY_STATE::END; ++i)
	{
		//콜백함수가 등록되어있고 마우스 버튼이 눌렸을 경우
		if (nullptr != m_arrpFuncLBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(eKEY::LBTN)))
			m_arrpFuncLBTNCallback[i](_Collider->GetOwner());

		if (nullptr != m_arrpFuncRBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(eKEY::RBTN)))
			m_arrpFuncRBTNCallback[i](_Collider->GetOwner());
	}
}

void cScript_MouseCursor::CalcPos()
{
	m_v2CursorPos = cKeyMgr::GetInst()->GetMouseLocalPos();

	//현재 커서의 위치에서 해상도의 반만큼 빼준 후 NDC 좌표와 일치시킨다
	m_v2CursorPos -= g_GlobalVal.v2Res * 0.5f;

	const Matrix& matView = cRenderMgr::GetInst()->GetCurCamera()->GetViewMatrix();
	//뷰 매트릭스의 위치 행을 반전시켜 주면 이동행렬을 얻을 수 있음.
	Vec3 Pos = -matView.Translation();

	m_v2CursorPos.x += Pos.x;
	m_v2CursorPos.y += Pos.y;

	GetOwner()->Transform().SetRelativePos(Vec3(m_v2CursorPos, 0.f));
}
