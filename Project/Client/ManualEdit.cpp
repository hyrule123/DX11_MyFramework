#include "pch.h"
#include "ManualEdit.h"

#include <Engine/CResMgr.h>
#include <Engine/CScriptMgr.h>

//string Keys
#include <Engine/strKey_Default.h>
#include <Script/strKey_Script.h>
#include <Script/strKey_Texture.h>
#include <Script/strKey_Shader.h>
#include "strKey_Prefab.h"

//Components
#include <Engine/CCollider2D_Rect.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CScriptHolder.h>

//Scripts
#include <Script/define_SCUnit.h>
#include <Script/CScript_FSM_Idle.h>
#include <Script/CScript_FSM_Attack.h>
#include <Script/CScript_FSM_Move_Ground.h>
#include <Script/CScript_FSM_Death.h>
#include <Script/CScript_SCEntity.h>

void ManualEdit::Edit()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//Marine
	{
		//Animation
		string strKey = strKey_TEXTURE::TERRAN::MARINE_BMP;
		MarineAnim_Save(strKey);
		LoadAnim(strKey);

		//Prefab
		strKey = strKey_RES_PREFAB::MARINE;
		MarinePrefab_Save(strKey);
		LoadPrefab(strKey);

		//Instantiate
		
	}
}


void ManualEdit::MarineAnim_Save(const string& _strKey)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::MARINE_BMP);
	assert(nullptr != pTex);


	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
	
	Atlas->SetAtlasTexture(pTex);

	using namespace FSM_SCUnit;
	Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);

	//Idle
	Atlas->AddAnim2D_SC_Redundant(strKey_Anim::IDLE, 0u, 1u, 1.f);

	//Attack Begin
	Atlas->AddAnim2D_SC_Redundant(strKey_Anim::ATTACK_BEGIN_END, 1u, 2u, 0.5f);

	//Move
	Atlas->AddAnim2D_SC_Redundant(strKey_Anim::MOVE, 4u, 9u, 1.f);

	//Attack
	vector<UINT> row = { 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 2u, 2u, 2u, 2u };
	Atlas->AddAnim2D_vecRowIndex(strKey_Anim::ATTACK, row, 0.6f);

	row.clear();

	//Death
	row = { 13u, 27u, 41u, 55u, 69u, 84u, 98u, 112u };
	Atlas->AddAnim2D(strKey_Anim::DEATH, row, 1.f);

	string strKey = strKey_TEXTURE::TERRAN::MARINE_BMP;
	
	Atlas->Save(strKey);
}

void ManualEdit::MarinePrefab_Save(const string& _strKey)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	CGameObject* pObj = new CGameObject;

	//Collider
	{
		CCollider2D_Rect* pCol = new CCollider2D_Rect;
		pObj->AddComponent(pCol);

		pCol->SetFollowTransformSize(false);
		pCol->SetCollSize(Vec2(17.f, 20.f));
	}

	//CAnimator2D
	{
		CAnimator2D* pAnim = new CAnimator2D;
		pObj->AddComponent(pAnim);

		Ptr<CAnim2DAtlas> pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(strKey_TEXTURE::TERRAN::MARINE_BMP);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAtlas);
	}

	//MeshRender
	{
		CMeshRender* pRenderCom = new CMeshRender;
		pObj->AddComponent(pRenderCom);

		//Material
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetKey(strKey_RES_PREFAB::MARINE);//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<CGraphicsShader> pShader = pResMgr->FindRes<CGraphicsShader>(strKey_RES_SHADER::GRAPHICS::SCUNITGROUND);
		pMtrl->SetShader(pShader);
		
		//Mesh
		Ptr<CMesh> pMesh = pResMgr->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}

	//Script
	{
		CScriptMgr* pScriptMgr = CScriptMgr::GetInst();

		CScript_SCEntity* pSCEntity = static_cast<CScript_SCEntity*>(pScriptMgr->GetNewScript(strKey_SCRIPTS::SCENTITY));
		pObj->AddScript(pSCEntity);
		
		CScript_FSM_Idle* pFSMIdle = static_cast<CScript_FSM_Idle*>(pScriptMgr->GetNewScript(strKey_SCRIPTS::FSM_IDLE));
		pObj->AddScript(pFSMIdle);

		CScript_FSM_Move_Ground* pFSMGround = GET_SCRIPT(CScript_FSM_Move_Ground, strKey_SCRIPTS::FSM_MOVE_GROUND);
		pObj->AddScript(pFSMGround);

		CScript_FSM_Attack* pFSMAttack = GET_SCRIPT(CScript_FSM_Attack, strKey_SCRIPTS::FSM_ATTACK);
		pObj->AddScript(pFSMAttack);


	}
	

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetKey(_strKey);
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(_strKey);
}

void ManualEdit::LoadAnim(const string& _strKey)
{
	Ptr<CAnim2DAtlas> pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(_strKey);
	assert(nullptr != pAtlas);
}

void ManualEdit::LoadPrefab(const string& _strKey)
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->Load<CPrefab>(_strKey);
	assert(nullptr != pPrefab);
}



//CResMgr* pResMgr = CResMgr::GetInst();
//Reaver
//{
//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
//	Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(strKey_TEXTURE::)));

//	Atlas->SetNewAnimUV_SC_Redundant(9u, 0u, 9u);
//	Atlas->AddAnim2D_SC_Redundant("MOVE", 0u, 9u, 1.f);

//	pResMgr->AddRes<CAnim2DAtlas>(string(RES_TEXTURE::), Atlas);
//}

//Corsair
//{
//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
//	Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(string(RES_TEXTURE::CORSAIR_BMP)));

//	Atlas->SetNewAnimUV(17u, 5u, 0u, 17u, 0u, 5u);
//	Atlas->AddAnim2D("MOVE", 0u, 17u, 0u, 5u, 0.3f, eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP);

//	pResMgr->AddRes<CAnim2DAtlas>(string(RES_TEXTURE::CORSAIR_BMP), Atlas);
//}
