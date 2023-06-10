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

namespace strKey_SCEntity
{
	STRKEY_DECLARE(m_eRace);
	STRKEY_DECLARE(m_uPopulation);
	STRKEY_DECLARE(m_eMoveType);
	STRKEY_DECLARE(m_eUnitSize);
	STRKEY_DECLARE(m_uSightRange);
	STRKEY_DECLARE(m_uCostMineral);
	STRKEY_DECLARE(m_uCostGas);
	STRKEY_DECLARE(m_fBaseBuildTime);
	STRKEY_DECLARE(m_strProdBuildingName);
	STRKEY_DECLARE(m_eKeyShortcut);
	STRKEY_DECLARE(m_flagTech);
};

CScript_SCEntity::CScript_SCEntity(const string& _strKey)
	: CScript(_strKey)
	, m_eRace()
	, m_uPopulation()
	, m_eMoveType()
	, m_eUnitSize()
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
	if (false == CScript::SaveJson(_pJVal))
		return false;

	Json::Value& jVal = *_pJVal;

	jVal[strKey_SCEntity::m_eRace] = (int)m_eRace;
	jVal[strKey_SCEntity::m_uPopulation] = (UINT)m_uPopulation;
	jVal[strKey_SCEntity::m_eMoveType] = (int)m_eMoveType;
	jVal[strKey_SCEntity::m_eUnitSize] = (int)m_eUnitSize;
	jVal[strKey_SCEntity::m_uSightRange] = (UINT)m_uSightRange;
	jVal[strKey_SCEntity::m_uCostMineral] = (UINT)m_uCostMineral;
	jVal[strKey_SCEntity::m_uCostGas] = (UINT)m_uCostGas;
	jVal[strKey_SCEntity::m_fBaseBuildTime] = Pack_float_int(m_fBaseBuildTime).i;
	jVal[strKey_SCEntity::m_strProdBuildingName] = m_strProdBuildingName;
	jVal[strKey_SCEntity::m_eKeyShortcut] = (UINT)m_eKeyShortcut;
	jVal[strKey_SCEntity::m_flagTech] = (UINT)m_flagTech;

	return true;
}

bool CScript_SCEntity::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;

	else if (false == CScript::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	try
	{
		m_eRace = (eSCUNIT_UNIT_RACE)jVal[strKey_SCEntity::m_eRace].asInt();
		m_uPopulation = jVal[strKey_SCEntity::m_uPopulation].asUInt();
		m_eMoveType = (eSCUNIT_MOVE_TYPE)jVal[strKey_SCEntity::m_eMoveType].asInt();
		m_eUnitSize = (eSCUNIT_SIZE)jVal[strKey_SCEntity::m_eUnitSize].asInt();
		m_uSightRange = jVal[strKey_SCEntity::m_uSightRange].asUInt();
		m_uCostMineral = jVal[strKey_SCEntity::m_uCostMineral].asUInt();
		m_uCostGas = jVal[strKey_SCEntity::m_uCostGas].asUInt();
		m_fBaseBuildTime = Pack_float_int(jVal[strKey_SCEntity::m_fBaseBuildTime].asInt()).f;
		m_strProdBuildingName = jVal[strKey_SCEntity::m_strProdBuildingName].asString();
		m_eKeyShortcut = (eKEY)jVal[strKey_SCEntity::m_eKeyShortcut].asInt();
		m_flagTech = jVal[strKey_SCEntity::m_flagTech].asUInt();
	}
	catch (const std::runtime_error& err)
	{
		ERROR_MESSAGE(err.what());
		return false;
	}

	return true;
}

void CScript_SCEntity::init()
{
	//유닛은 기본적으로 전부 회전이 존재하지 않음.
	Transform()->SetLockRotation(true);

	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	//GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);
}

void CScript_SCEntity::start()
{
	assert(nullptr != ScriptHolder()->Animator2D());
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

void CScript_SCEntity::Attack(CGameObject* _pTarget)
{

}

