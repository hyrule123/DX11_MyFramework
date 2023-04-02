#include "pch.h"
#include "CSCEntity.h"

#include "CScript_Bullet.h"

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

#include "defineScript.h"
#include "CFState_SCUnitIdle.h"
#include "CFState_SCUnitMove_Ground.h"
//TODO : 
//#include "CScript_SCGroundUnitAttack.h"
//#include "CScript_SCGroundUnitDeath.h"


CSCEntity::CSCEntity(std::type_index _typeIdx, UINT _eNumState)
	: CFStateMgr(_typeIdx, _eNumState)
	, m_eUnitType()
	, m_uPopulation()

	, m_eMoveType()
	, m_eUnitSizeType()
	, m_uSightRange(5u)
	, m_uCostMineral(50u)
	, m_uCostGas()

	, m_fBaseBuildTime()
	, m_ProdSlot()
	, m_eKeyShortcut()
	, m_flagTech()
	
	//, m_ColorKey(0.f, 0.f, 0.f, 0.f)
{
}

CSCEntity::~CSCEntity()
{
}

void CSCEntity::initFSM()
{
	GetOwner()->Transform()->SetLockRotation(true);
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	//GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);
}

void CSCEntity::OnCollision(CCollider* _pCol, const Vec3& _v3HitPoint)
{
	//커서가 자신과 충돌중임을 확인했을 경우
	if (iLayerCursor == _pCol->GetOwner()->GetLayer())
	{
		//마우스 커서가 한번 클릭됐을 경우
		if (KEY_TAP(eKEY::LBTN))
		{

			//m_bSelected = true;
		}
	}
}

