#include "pch.h"
#include "cScript_SCEntity.h"

#include <Engine/cGameObject.h>

#include <Engine/IRenderer.h>
#include <Engine/cMaterial.h>
#include <Engine/cTransform.h>

#include <Engine/cTimeMgr.h>
#include <Engine/cKeyMgr.h>

//Prefab instantiate
#include <Engine/cResMgr.h>
#include <Engine/CEventMgr.h>


#include <Engine/ICollider.h>
#include <Engine/define.h>

#include <Engine/jsoncpp.h>

#include "defineScript.h"
//#include "cScript_FSM_Idle.h"
//#include "cScript_FSM_Move_Ground.h"
#include "define_SC.h"

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

cScript_SCEntity::cScript_SCEntity()
	: m_eRace()
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

cScript_SCEntity::~cScript_SCEntity()
{
}

bool cScript_SCEntity::SaveJson(Json::Value* _pJVal)
{
	if (false == IScript::SaveJson(_pJVal))
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

bool cScript_SCEntity::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
		return false;

	else if (false == IScript::LoadJson(_pJVal))
		return false;

	const Json::Value& jVal = *_pJVal;

	try
	{
		m_eRace = (SC::eRACE)jVal[strKey_SCEntity::m_eRace].asInt();
		m_uPopulation = jVal[strKey_SCEntity::m_uPopulation].asUInt();
		m_eMoveType = (SC::eUNIT_MOVE_TYPE)jVal[strKey_SCEntity::m_eMoveType].asInt();
		m_eUnitSize = (SC::eUNIT_SIZE_TYPE)jVal[strKey_SCEntity::m_eUnitSize].asInt();
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
		ERROR_MESSAGE_A(err.what());
		return false;
	}

	return true;
}

void cScript_SCEntity::Init()
{
	//유닛은 기본적으로 전부 회전이 존재하지 않음.
	GetOwner()->Transform().SetLockRotation(true);

	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	//GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);
}

void cScript_SCEntity::OnCollision(ICollider* _pCol, const Vec3& _v3HitPoint)
{
	//커서가 자신과 충돌중임을 확인했을 경우
	if (SC::LAYER_INFO::MouseCursor == _pCol->GetOwner()->GetLayer())
	{
		//마우스 커서가 한번 클릭됐을 경우
		if (KEY_DOWN(eKEY::LBTN))
		{
			
		}
	}
}

void cScript_SCEntity::Attack(cGameObject* _pTarget)
{

}

