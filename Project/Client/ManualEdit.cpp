#include "pch.h"
#include "ManualEdit.h"

#include <Engine/CResMgr.h>
#include <Engine/CScriptMgr.h>
#include <Engine/CRandMgr.h>
#include <Engine/EventDispatcher.h>

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
#include <Engine/CPrefab.h>
#include <Engine/CTransform.h>

//Scripts
#include <Script/SC_Func.h>
#include <Script/define_SC.h>
#include <Script/CScript_FSM_Idle.h>
#include <Script/CScript_FSM_Attack.h>
#include <Script/CScript_FSM_Move_Ground.h>
#include <Script/CScript_FSM_Death.h>
#include <Script/CScript_SCEntity.h>
#include <Script/CScript_FSM_Building_Prod.h>

//HLSL Header
#include <Engine/S_H_SCUnitGround.hlsli>

void ManualEdit::Edit()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//Terran-Common Animations
	Terran_CommonAnim_Save();
	

	//Marine
	{
		//Animation
		string strKey = strKey_TEXTURE::TERRAN::MARINE_BMP;
		MarineAnim_Save(strKey);
		LoadAnim(strKey);

		//Prefab
		strKey = SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE);
		MarinePrefab_Save(strKey);
		LoadPrefab(strKey);
	}

	//Command Center
	{
		//Animation
		CommandCenter_Anim_Save();
		LoadAnim(strKey_TEXTURE::TERRAN::COMMANDCENTER_CONTROL__BMP);
		LoadAnim(strKey_TEXTURE::TERRAN::CONSTRUCTION_LARGE_TBLDLRG__BMP);

		string strKey = SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER);
		CommandCenter_Prefab_Save(strKey);
	}

	//Mineral
	{
		Resources_Prefab_Save();
	}
}

void ManualEdit::TestCreate()
{
	for (int i = 0; i < 1; ++i)
	{
		Ptr<CPrefab> MarinePrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));
		CGameObject* Marine = MarinePrefab->Instantiate();

		float randx = CRandMgr::GetInst()->GetRand<float>(-640.f, 640.f);
		float randy = CRandMgr::GetInst()->GetRand<float>(-320.f, 320.f);
		EventDispatcher::SpawnGameObject(Marine, Vec3(randx, randy, 0.f), 1);

		CScript_FSM_Move_Ground* pMoveGround = static_cast<CScript_FSM_Move_Ground*>(Marine->ScriptHolder()->FindScript(strKey_SCRIPT::FSM_MOVE_GROUND));

		pMoveGround->SetSpeed(100.f);
	}

	{
		Ptr<CPrefab> CCPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
		CGameObject* CommandCenter = CCPrefab->Instantiate();
		Ptr<CMaterial> pMtrl = CommandCenter->RenderComponent()->GetCurMaterial();

		EventDispatcher::SpawnGameObject(CommandCenter, Vec3(0.f, 0.f, 0.f), SC::LAYER_INFO::GroundUnitMain);
	}

	{
		Ptr<CPrefab> pPrefab = CResMgr::GetInst()->Load<CPrefab>(SC::strKey_PREFAB::MINERAL);

		CGameObject* Mineral = pPrefab->Instantiate();
		
		EventDispatcher::SpawnGameObject(Mineral, Vec3(100.f, 100.f, 100.f), SC::LAYER_INFO::GroundUnitMain);
	}
}

void ManualEdit::Terran_CommonAnim_Save()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//대형건물 건설 애니메이션(초반부)
	//기본 시간은 100초이므로 이걸 재생속도 배율을 늘려서 사용할것(34초 -> 0.34f)
	//또한 애니메이션도 1개 뿐이므로 동일한 키값을 사용
	{
		Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::CONSTRUCTION_LARGE_TBLDLRG__BMP);
		assert(nullptr != pTex);

		Ptr<CAnim2DAtlas> pAnim = new CAnim2DAtlas;
		//pAnim->SetKey(pTex->GetKey());
		pAnim->SetAtlasTexture(pTex);
		

		//UV 쪼개기 작업
		pAnim->SetNewAnimUV(3u, 1u);

		vector<UINT> vecFrame;
		vecFrame.push_back(0);
		vecFrame.push_back(1);
		vecFrame.push_back(2);
		pAnim->AddAnim2D(SC::strKey_Anim::Terran::CONSTRUCTION_LARGE, vecFrame, 100.f);

		
		pAnim->Save(pAnim->GetKey());
	}
}


void ManualEdit::MarineAnim_Save(const string& _strKey)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::MARINE_BMP);
	assert(nullptr != pTex);

	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
	
	Atlas->SetAtlasTexture(pTex);

	using namespace SC::FSM;
	Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);

	//Idle
	Atlas->AddAnim2D_SC_Redundant(strKey_Anim::IDLE, 0u, 1u, 1.f);

	//Attack Begin
	Atlas->AddAnim2D_SC_Redundant(strKey_Anim::ATTACK_BEGIN_END, 1u, 2u, 1.f);

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
	pObj->SetName(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));
	pObj->SetLayer(SC::LAYER_INFO::GroundUnitMain);

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
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<CGraphicsShader> pShader = pResMgr->FindRes<CGraphicsShader>(strKey_SHADER::GRAPHICS::SCUNITGROUND);
		pMtrl->SetShader(pShader);
		
		//Mesh
		Ptr<CMesh> pMesh = pResMgr->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}

	//Script
	{
		CScriptMgr* pScriptMgr = CScriptMgr::GetInst();

		CScript_SCEntity* pSCEntity = static_cast<CScript_SCEntity*>(pScriptMgr->GetNewScript(strKey_SCRIPT::SCENTITY));
		pObj->AddScript(pSCEntity);
		
		CScript_FSM_Idle* pFSMIdle = static_cast<CScript_FSM_Idle*>(pScriptMgr->GetNewScript(strKey_SCRIPT::FSM_IDLE));
		pObj->AddScript(pFSMIdle);

		CScript_FSM_Move_Ground* pFSMGround = GET_SCRIPT(CScript_FSM_Move_Ground, strKey_SCRIPT::FSM_MOVE_GROUND);
		pObj->AddScript(pFSMGround);

		CScript_FSM_Attack* pFSMAttack = GET_SCRIPT(CScript_FSM_Attack, strKey_SCRIPT::FSM_ATTACK);
		pObj->AddScript(pFSMAttack);
	}
	
	
	

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(_strKey);
}



void ManualEdit::CommandCenter_Anim_Save()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//메인 스프라이트
	{
		Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::COMMANDCENTER_CONTROL__BMP);
		assert(nullptr != pTex);
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetKey(pTex->GetKey());

		Atlas->SetAtlasTexture(pTex);

		Atlas->SetNewAnimUV(6u, 1u);

		vector<UINT> vecIdx;
		vecIdx.push_back(0);

		Atlas->AddAnim2D(SC::FSM::strKey_Anim::IDLE, vecIdx, 0.1f);
		vecIdx.clear();

		//고유 건설 애니메이션
		vecIdx.push_back(1);
		vecIdx.push_back(2);
		Atlas->AddAnim2D(SC::FSM::strKey_Anim::IN_CONSTRUCTION, vecIdx, 100.f);
		vecIdx.clear();

		//이륙
		vecIdx.push_back(3);
		vecIdx.push_back(4);
		vecIdx.push_back(5);
		Atlas->AddAnim2D(SC::FSM::strKey_Anim::LIFTOFF, vecIdx, 1.f);

		Atlas->Save(Atlas->GetKey());
	}

	//생산 스프라이트(controlt)
	//텍스처 한 장이므로 굳이 할필요가 없다
	{
		Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::COMMANDCENTER_PROD_CONTROLT__BMP);
		assert(nullptr != pTex);
		//Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		//Atlas->SetKey(pTex->GetKey());
	}

}

void ManualEdit::CommandCenter_Prefab_Save(const string& _strKey)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	CGameObject* pObj = new CGameObject;
	pObj->SetName(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
	pObj->SetLayer(SC::LAYER_INFO::GroundUnitMain);

	//Collider
	{
		CCollider2D_Rect* pCol = new CCollider2D_Rect;
		pObj->AddComponent(pCol);

		SC_Func::SetSCBuildingSize(pCol, 3, 2, Vec4(6.f, 5.f, 7.f, 6.f));
	}

	//CAnimator2D
	{
		CAnimator2D* pAnim = new CAnimator2D;
		pObj->AddComponent(pAnim);

		Ptr<CAnim2DAtlas> pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(strKey_TEXTURE::TERRAN::COMMANDCENTER_CONTROL__BMP);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAtlas);

		
		pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(strKey_TEXTURE::TERRAN::CONSTRUCTION_LARGE_TBLDLRG__BMP);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_1, pAtlas);
	}

	//MeshRender
	{
		CMeshRender* pRenderCom = new CMeshRender;
		pObj->AddComponent(pRenderCom);

		//Material
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<CGraphicsShader> pShader = pResMgr->FindRes<CGraphicsShader>(strKey_SHADER::GRAPHICS::BUILDINGSTRUCTURE);
		pMtrl->SetShader(pShader);

		Ptr<CTexture> pFlash = CResMgr::GetInst()->Load<CTexture>(strKey_TEXTURE::TERRAN::COMMANDCENTER_PROD_CONTROLT__BMP);

		pMtrl->SetTexParam((eMTRLDATA_PARAM_TEX)iTexProdIdx, pFlash);

		//Mesh
		Ptr<CMesh> pMesh = pResMgr->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}

	//Script
	{
		CScriptMgr* pScriptMgr = CScriptMgr::GetInst();

		CScript_SCEntity* pEntity = static_cast<CScript_SCEntity*>(pScriptMgr->GetNewScript(strKey_SCRIPT::SCENTITY));
		pObj->AddScript(pEntity);

		CScript_FSM_Building_Prod* pFSMBuilding = static_cast<CScript_FSM_Building_Prod*>(pScriptMgr->GetNewScript(strKey_SCRIPT::FSM_BUILDING_PROD));
		pObj->AddScript(pFSMBuilding);
	}

	pObj->SetName(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetKey(_strKey);
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(_strKey);
}

void ManualEdit::Resources_Anim_Save()
{
	//CResMgr* pResMgr = CResMgr::GetInst();

	////Mineral01
	//{
	//	Ptr<CTexture> Mineral01 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN01_BMP);
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

	//	Atlas->SetAtlasTexture(Mineral01);
	//	Atlas->SetNewAnimUV(4u, 1u);

	//	vector<UINT> vecFrame;
	//	vecFrame.push_back(0u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_WEAR_0, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(1u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_2, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(2u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_1, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(3u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_0, vecFrame, 0.1f);

	//	Atlas->Save(strKey_TEXTURE::NEUTRAL::MIN01_BMP);
	//}

	////Mineral02
	//{
	//	Ptr<CTexture> Mineral01 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN02_BMP);
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

	//	Atlas->SetAtlasTexture(Mineral01);
	//	Atlas->SetNewAnimUV(4u, 1u);

	//	vector<UINT> vecFrame;
	//	vecFrame.push_back(0u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_3, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(1u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_2, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(2u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_1, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(3u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_0, vecFrame, 0.1f);

	//	Atlas->Save(strKey_TEXTURE::NEUTRAL::MIN01_BMP);
	//}

	////Mineral03
	//{
	//	Ptr<CTexture> Mineral01 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN03_BMP);
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

	//	Atlas->SetAtlasTexture(Mineral01);
	//	Atlas->SetNewAnimUV(4u, 1u);

	//	vector<UINT> vecFrame;
	//	vecFrame.push_back(0u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_3, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(1u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_2, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(2u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_1, vecFrame, 0.1f);

	//	vecFrame.clear();
	//	vecFrame.push_back(3u);
	//	Atlas->AddAnim2D(SC::strKey_Anim::Neutral::MINERAL_REMAIN_0, vecFrame, 0.1f);

	//	Atlas->Save(strKey_TEXTURE::NEUTRAL::MIN01_BMP);
	//}
}

void ManualEdit::Resources_Prefab_Save()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	CGameObject* pObj = new CGameObject;
	pObj->SetName(SC::GetUnitName(SC::eUNIT_ID::MINERAL_FIELD_TYPE_1));
	pObj->SetLayer(SC::LAYER_INFO::GroundUnitMain);



	//Collider
	{
		CCollider2D_Rect* pCol = new CCollider2D_Rect;
		pObj->AddComponent(pCol);

		SC_Func::SetSCBuildingSize(pCol, 2, 2, Vec4(0.f));
	}


	//MeshRender
	{
		CMeshRender* pRenderCom = new CMeshRender;
		pObj->AddComponent(pRenderCom);


		//Material
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::MINERAL_FIELD_TYPE_1));//프리팹 키와 동일한 키를 사용

		pRenderCom->SetMaterial(pMtrl);

		Ptr<CGraphicsShader> pShader = pResMgr->Load<CGraphicsShader>(strKey_SHADER::GRAPHICS::MINERAL
		);
		pMtrl->SetShader(pShader);

		//미네랄 텍스처 등록
		Ptr<CTexture> Mineral1 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN01_BMP);
		Ptr<CTexture> Mineral2 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN02_BMP);
		Ptr<CTexture> Mineral3 = pResMgr->Load<CTexture>(strKey_TEXTURE::NEUTRAL::MIN03_BMP);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, Mineral1);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_1, Mineral2);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_2, Mineral3);

		//Transform
		{
			Vec2 size = Mineral1->GetSize();
			//4장 고정이므로 1/4
			size.x *= 0.25f;

			pObj->Transform()->SetSize(Vec3(size, 1.f));
		}


		//Mesh
		Ptr<CMesh> pMesh = pResMgr->FindRes<CMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}



	//Script
	{
		CScriptMgr* pScriptMgr = CScriptMgr::GetInst();

		pObj->AddScript(pScriptMgr->GetNewScript(strKey_SCRIPT::MINERAL));
	}


	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(SC::strKey_PREFAB::MINERAL);
}

Ptr<CAnim2DAtlas> ManualEdit::LoadAnim(const string& _strKey)
{
	Ptr<CAnim2DAtlas> pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(_strKey);
	assert(nullptr != pAtlas);

	return pAtlas;
}

Ptr<CPrefab> ManualEdit::LoadPrefab(const string& _strKey)
{
	Ptr<CPrefab> pPrefab = CResMgr::GetInst()->Load<CPrefab>(_strKey);
	assert(nullptr != pPrefab);

	return pPrefab;
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
