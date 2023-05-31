#include "pch.h"
#include "CScript_SCEntity.h"

#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CTransform.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>

//Prefab instantiate
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CScriptHolder.h>

#include <Engine/CCollider.h>
#include <Engine/define.h>

#include <Engine/jsoncpp.h>

#include "defineScript.h"
#include "CScript_FSM_Idle.h"
#include "CScript_FSM_Move_Ground.h"


CScript_SCEntity::CScript_SCEntity(const string& _strKey)
	: CScript(_strKey)
	, m_eUnitType()
	, m_uPopulation()
	, m_eMoveType()
	, m_eUnitSizeType()
	, m_uSightRange(5u)
	, m_uCostMineral(50u)
	, m_uCostGas()
	, m_fBaseBuildTime()
	, m_eKeyShortcut()
	, m_flagTech()
{
}

CScript_SCEntity::~CScript_SCEntity()
{
}

bool CScript_SCEntity::SaveJson(Json::Value* _pJVal)
{
	

	return false;
}

bool CScript_SCEntity::LoadJson(Json::Value* _pJVal)
{
	return false;
}

void CScript_SCEntity::init()
{
	//유닛은 기본적으로 전부 회전이 존재하지 않음.
	Transform()->SetLockRotation(true);

	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	//GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);
}

void CScript_SCEntity::OnCollision(CCollider* _pCol, const Vec3& _v3HitPoint)
{
	//커서가 자신과 충돌중임을 확인했을 경우
	if (iLayerCursor == _pCol->GetOwner()->GetLayer())
	{
		//마우스 커서가 한번 클릭됐을 경우
		if (KEY_TAP(eKEY::LBTN))
		{
			
		}
	}
}

