#include "pch.h"
#include "ManualEdit.h"


#include <Engine/cComMgr.h>
#include <Engine/RandGen.h>
#include <Engine/EventDispatcher.h>

//Resource
#include <Engine/cResMgr.h>
#include <Engine/cPrefab.h>
#include <Engine/cAnim2DAtlas.h>

#include <Engine/cCom_Renderer_TilemapComplete.h>

//string Keys
#include <Engine/strKey_Default.h>
#include <Script/strKey_Component.h>
#include <Script/strKey_Texture.h>
#include <Script/strKey_Shader.h>
#include "strKey_Prefab.h"

//Components
#include <Engine/cCom_Animator2D.h>
#include <Engine/cCom_Coll2D_Rect.h>
#include <Engine/cCom_Coll2D_Rect.h>
#include <Engine/cCom_Renderer_Default.h>
#include <Script/cCom_Renderer_TilemapSC.h>
#include <Engine/cPrefab.h>
#include <Engine/cTransform.h>

//Scripts
#include <Script/SC_Func.h>
#include <Script/define_SC.h>

#include <Script/CScript_SCEntity.h>
//#include <Script/CScript_FSM_Building_Prod.h>
//#include <Script/CScript_FSM_Idle.h>
//#include <Script/cScript_FSM_Attack.h>
//#include <Script/CScript_FSM_Move_Ground.h>
//#include <Script/CScript_FSM_Death.h>

//HLSL Header
#include <HLSL/S_H_SCUnitGround.hlsli>

#include <Engine/cGameObject.h>

#include <Script/strKey_Component.h>

void ManualEdit::Edit()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	//Terran-Common Animations
	Terran_CommonAnim_Save();

	//Marine
	{
		//Animation
		const char* strKey = strKey_Texture::SC::Terran::marine_bmp;
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
		LoadAnim(strKey_Texture::SC::Terran::CommandCenter_control__bmp);
		LoadAnim(strKey_Texture::SC::Terran::Construction_Large_tbldlrg__bmp);

		string strKey = SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER);
		CommandCenter_Prefab_Save(strKey);
	}

	//Mineral
	{
		Mineral_Prefab_Save();
	}

	//Gas
	{
		Vespene_Anim_Save();
		Vespene_Prefab_Save();
	}

	//Map
	{
		Map_Prefab_Save();
	}
}

void ManualEdit::TestCreate()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	for (int i = 0; i < 1; ++i)
	{
		Ptr<cPrefab> MarinePrefab = cResMgr::GetInst()->Load<cPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));
		cGameObject* Marine = MarinePrefab->Instantiate();

		float randx = RandGen::GetRand<float>(-640.f, 640.f);
		float randy = RandGen::GetRand<float>(-320.f, 320.f);
		EventDispatcher::SpawnGameObject(Marine, Vec3(randx, randy, 0.f));

		//CScript_FSM_Move_Ground* pMoveGround = static_cast<CScript_FSM_Move_Ground*>(Marine->ScriptHolder()->FindScript(strKey_Component::CScript_FSM_Move_Ground));

		//pMoveGround->SetSpeed(100.f);
	}

	{
		Ptr<cPrefab> CCPrefab = pResMgr->Load<cPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
		cGameObject* CommandCenter = CCPrefab->Instantiate();

		
		Ptr<cMaterial> pMtrl = CommandCenter->GetComponent<cCom_Renderer_Default>()->GetCurMaterial();

		EventDispatcher::SpawnGameObject(CommandCenter, Vec3(0.f, 0.f, 0.f), SC::LAYER_INFO::GroundUnitMain);
	}


	//Mineral
	{
		Ptr<cPrefab> pPrefab = pResMgr->Load<cPrefab>(SC::strKey_PREFAB::MINERAL);

		//EventDispatcher::SpawnPrefab(pPrefab, Vec2(100.f, 100.f));
	}

	//Vespene
	{
		Ptr<cPrefab> pPrefab = pResMgr->Load<cPrefab>(SC::GetUnitName(SC::eUNIT_ID::VESPENE_GEYSER));
		EventDispatcher::SpawnPrefab2D(pPrefab, Vec2(-100.f, 100.f));
	}



}

void ManualEdit::Terran_CommonAnim_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	//대형건물 건설 애니메이션(초반부)
	//기본 시간은 100초이므로 이걸 재생속도 배율을 늘려서 사용할것(34초 -> 0.34f)
	//또한 애니메이션도 1개 뿐이므로 동일한 키값을 사용
	{
		Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::Terran::Construction_Large_tbldlrg__bmp);
		assert(nullptr != pTex);

		Ptr<cAnim2DAtlas> pAnim = new cAnim2DAtlas;
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
	cResMgr* pResMgr = cResMgr::GetInst();

	Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::Terran::marine_bmp);
	assert(nullptr != pTex);

	Ptr<cAnim2DAtlas> Atlas = new cAnim2DAtlas;
	
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

	string strKey = strKey_Texture::SC::Terran::marine_bmp;
	
	Atlas->Save(strKey);
}

void ManualEdit::MarinePrefab_Save(const string& _strKey)
{
	cResMgr* pResMgr = cResMgr::GetInst();

	cGameObject* pObj = new cGameObject;
	pObj->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));
	pObj->SetLayer(SC::LAYER_INFO::GroundUnitMain);

	//Collider
	{	
		cCom_Coll2D_Rect* pCol = pObj->AddComponent<cCom_Coll2D_Rect>();

		pCol->SetFollowTransformSize(false);
		pCol->SetCollSize(Vec2(17.f, 20.f));
	}

	//cCom_Animator2D
	{
		cCom_Animator2D* pAnim = pObj->AddComponent<cCom_Animator2D>();


		Ptr<cAnim2DAtlas> pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(strKey_Texture::SC::Terran::marine_bmp);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAtlas);
	}

	//MeshRender
	{
		cCom_Renderer_Default* pRenderCom = pObj->AddComponent<cCom_Renderer_Default>();

		//Material
		Ptr<cMaterial> pMtrl = new cMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<cGraphicsShader> pShader = pResMgr->Load<cGraphicsShader>(strKey_Shader::Graphics::SCUnitGround);
		pMtrl->SetShader(pShader);
		
		//Mesh
		Ptr<cMesh> pMesh = pResMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}
	
	//Script
	{
		cScript_SCEntity* pSCEntity = static_cast<cScript_SCEntity*>(cComMgr::GetInst()->GetNewCom(strKey_Com::cScript_SCEntity));
		pObj->AddComponent(pSCEntity);
		
		//pObj->AddComponent<
		//CScript_FSM_Idle* pFSMIdle = static_cast<CScript_FSM_Idle*>(cComMgr::GetNewScript(strKey_Component::CScript_FSM_Idle));
		//pObj->AddScript(pFSMIdle);

		//CScript_FSM_Move_Ground* pFSMGround = GET_NEW_SCRIPT(CScript_FSM_Move_Ground);
		//pObj->AddScript(pFSMGround);

		//cScript_FSM_Attack* pFSMAttack = GET_NEW_SCRIPT(cScript_FSM_Attack);
		//pObj->AddScript(pFSMAttack);
	}
	

	Ptr<cPrefab> pPrefab = new cPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(_strKey);
}



void ManualEdit::CommandCenter_Anim_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	//메인 스프라이트
	{
		Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::Terran::CommandCenter_control__bmp);
		assert(nullptr != pTex);
		Ptr<cAnim2DAtlas> Atlas = new cAnim2DAtlas;
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
		Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::Terran::CommandCenter_Prod_controlt__bmp);
		assert(nullptr != pTex);
		//Ptr<cAnim2DAtlas> Atlas = new cAnim2DAtlas;
		//Atlas->SetKey(pTex->GetKey());
	}
}

void ManualEdit::CommandCenter_Prefab_Save(const string& _strKey)
{
	cResMgr* pResMgr = cResMgr::GetInst();

	cGameObject* pObj = new cGameObject;
	pObj->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
	pObj->SetLayer(SC::LAYER_INFO::GroundUnitMain);

	//Collider
	{
		cCom_Coll2D_Rect* pCol = pObj->AddComponent<cCom_Coll2D_Rect>();

		SC_Func::SetSCBuildingSize(pCol, 3, 2, Vec4(6.f, 5.f, 7.f, 6.f));
	}

	//cCom_Animator2D
	{
		cCom_Animator2D* pAnim = pObj->AddComponent<cCom_Animator2D>();

		Ptr<cAnim2DAtlas> pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(strKey_Texture::SC::Terran::CommandCenter_control__bmp);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAtlas);

		
		pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(strKey_Texture::SC::Terran::Construction_Large_tbldlrg__bmp);
		pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_1, pAtlas);
	}

	//MeshRender
	{
		cCom_Renderer_Default* pRenderCom = pObj->AddComponent<cCom_Renderer_Default>();

		//Material
		Ptr<cMaterial> pMtrl = new cMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<cGraphicsShader> pShader = pResMgr->Load<cGraphicsShader>(strKey_Shader::Graphics::BuildingStructure);
		pMtrl->SetShader(pShader);

		Ptr<cTexture> pFlash = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::Terran::CommandCenter_Prod_controlt__bmp);

		pMtrl->SetTexParam((eMTRLDATA_PARAM_TEX)iTexProdIdx, pFlash);

		//Mesh
		Ptr<cMesh> pMesh = pResMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}

	//Script
	{
		pObj->AddComponent<cScript_SCEntity>();

		//CScript_FSM_Building_Prod* pFSMBuilding = static_cast<CScript_FSM_Building_Prod*>(cComMgr::GetNewScript(strKey_Component::CScript_FSM_Building_Prod));
		//pObj->AddScript(pFSMBuilding);
	}

	pObj->SetKey(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));

	Ptr<cPrefab> pPrefab = new cPrefab;
	pPrefab->SetKey(_strKey);
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(_strKey);
}


void ManualEdit::Mineral_Prefab_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	cGameObject* pObj = new cGameObject;
	pObj->SetKey(SC::GetUnitName(SC::eUNIT_ID::MINERAL_FIELD_TYPE_1));
	pObj->SetLayer(SC::LAYER_INFO::Resource);

	//Collider
	{
		cCom_Coll2D_Rect* pCol = pObj->AddComponent<cCom_Coll2D_Rect>();

		SC_Func::SetSCBuildingSize(pCol, 2, 1, Vec4(0.f));
	}


	//MeshRender
	{
		cCom_Renderer_Default* pRenderCom = pObj->AddComponent<cCom_Renderer_Default>();

		//Material
		Ptr<cMaterial> pMtrl = new cMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::MINERAL_FIELD_TYPE_1));//프리팹 키와 동일한 키를 사용

		pRenderCom->SetMaterial(pMtrl);

		Ptr<cGraphicsShader> pShader = pResMgr->Load<cGraphicsShader>(strKey_Shader::Graphics::Mineral
		);
		pMtrl->SetShader(pShader);

		//미네랄 텍스처 등록
		Ptr<cTexture> Mineral1 = pResMgr->Load<cTexture>(strKey_Texture::SC::Neutral::min01_bmp);
		Ptr<cTexture> Mineral2 = pResMgr->Load<cTexture>(strKey_Texture::SC::Neutral::min02_bmp);
		Ptr<cTexture> Mineral3 = pResMgr->Load<cTexture>(strKey_Texture::SC::Neutral::min03_bmp);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, Mineral1);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_1, Mineral2);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_2, Mineral3);

		//Transform
		{
			Vec2 size = Mineral1->GetSize();
			//4장 고정이므로 1/4
			size.x *= 0.25f;

			pObj->Transform().SetSize(Vec3(size, 1.f));
		}


		//Mesh
		Ptr<cMesh> pMesh = pResMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}



	//Script
	{
		pObj->AddComponent(cComMgr::GetInst()->GetNewCom(strKey_Com::cScript_Mineral));
	}


	Ptr<cPrefab> pPrefab = new cPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(SC::strKey_PREFAB::MINERAL);
}

void ManualEdit::Vespene_Anim_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	//일반 연기
	{
		Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::thingy::VSmoke_geysmok1__bmp);
		assert(nullptr != pTex);
		Ptr<cAnim2DAtlas> Atlas = new cAnim2DAtlas;
		Atlas->SetKey(pTex->GetKey());

		Atlas->SetAtlasTexture(pTex);

		Atlas->SetNewAnimUV(8u, 1u);

		vector<UINT> vecIdx;
		for (int i = 0; i < 8; ++i)
		{
			vecIdx.push_back(i);
		}
		
		Atlas->AddAnim2D(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1, vecIdx, 0.7f);
		vecIdx.clear();

		Atlas->Save(Atlas->GetKey());
	}

	//고갈 연기
	{
		Ptr<cTexture> pTex = cResMgr::GetInst()->Load<cTexture>(strKey_Texture::SC::thingy::VSmokeDeplete_geysmok4__bmp);
		assert(nullptr != pTex);
		Ptr<cAnim2DAtlas> Atlas = new cAnim2DAtlas;
		Atlas->SetKey(pTex->GetKey());

		Atlas->SetAtlasTexture(pTex);

		Atlas->SetNewAnimUV(8u, 1u);

		vector<UINT> vecIdx;
		for (int i = 0; i < 8; ++i)
		{
			vecIdx.push_back(i);
		}

		Atlas->AddAnim2D(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1_DEPLETE, vecIdx, 1.f);
		vecIdx.clear();

		Atlas->Save(Atlas->GetKey());
	}
}

void ManualEdit::Vespene_Prefab_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	cGameObject* pObj = new cGameObject;
	pObj->SetKey(SC::GetUnitName(SC::eUNIT_ID::VESPENE_GEYSER));
	pObj->SetLayer(SC::LAYER_INFO::Resource);

	//Collider
	{
		cCom_Coll2D_Rect* pCol = pObj->AddComponent< cCom_Coll2D_Rect>();

		pCol->SetFollowTransformSize(false);
		SC_Func::SetSCBuildingSize(pCol, 4, 2, Vec4(0.f, 0.f, 0.f, 0.f));
	}

	//Transform
	{
		pObj->Transform().SetSizeXY(Vec2(SC::UNIT_INFO::VespeneGeyser::Size));
	}


	//MeshRender
	{
		cCom_Renderer_Default* pRenderCom = pObj->AddComponent< cCom_Renderer_Default>();

		//Material
		Ptr<cMaterial> pMtrl = new cMaterial;
		pMtrl->SetKey(SC::GetUnitName(SC::eUNIT_ID::VESPENE_GEYSER));//프리팹 키와 동일한 키를 사용
		pRenderCom->SetMaterial(pMtrl);
		Ptr<cGraphicsShader> pShader = pResMgr->Load<cGraphicsShader>(strKey_Shader::Graphics::VespineGeyser);
		pMtrl->SetShader(pShader);

		Ptr<cTexture> VespeneTex = pResMgr->Load<cTexture>(strKey_Texture::SC::Neutral::geyser_bmp);
		assert(nullptr != VespeneTex);
		pMtrl->SetTexParam(eMTRLDATA_PARAM_TEX::_0, VespeneTex);

		//Mesh
		Ptr<cMesh> pMesh = pResMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT);
		pRenderCom->SetMesh(pMesh);
	}

	//Script
	{
		pObj->AddComponent<cScript_SCEntity>();

		//가스 스크립트 추가할것
	}


	//Child
	constexpr int numSmoke = 3;
	for(int i = 0; i < numSmoke; ++i)
	{	
		cGameObject* pChild = new cGameObject;
		pChild->SetKey(SC::strKey_PREFAB::VESPENE_SMOKE);
		pChild->SetLayer(SC::LAYER_INFO::Resource);

		pObj->AddChildGameObj(pChild);

		//Transform
		{
			//셋 다 동일한 프리팹 파일을 사용하기 때문에 현재 구조상 스크립트에서 위치를 직접 설정해줘야 함
			//pChild->Transform().SetRelativePosXY(Vec2(0.f, 0.f));
		}

		//cCom_Animator2D
		{
			cCom_Animator2D* pAnim = pChild->AddComponent<cCom_Animator2D>();

			Ptr<cAnim2DAtlas> pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(strKey_Texture::SC::thingy::VSmoke_geysmok1__bmp);
			pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_0, pAtlas);


			pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(strKey_Texture::SC::thingy::VSmokeDeplete_geysmok4__bmp);
			pAnim->AddAtlasTex(eMTRLDATA_PARAM_TEX::_1, pAtlas);
		}

		//MeshRender
		{
			cCom_Renderer_Default* pRenderCom = pChild->AddComponent< cCom_Renderer_Default>();

			//Material
			Ptr<cMaterial> pMtrl = new cMaterial;
			pMtrl->SetKey(SC::strKey_PREFAB::VESPENE_SMOKE);//프리팹 키와 똑같은 키를 사용
			pRenderCom->SetMaterial(pMtrl);
			Ptr<cGraphicsShader> pShader = pResMgr->FindRes<cGraphicsShader>(strKey_Shader::Graphics::SCUnitGround);
			pMtrl->SetShader(pShader);

			//Mesh
			Ptr<cMesh> pMesh = pResMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT);
			pRenderCom->SetMesh(pMesh);
		}

		//Script
		{
			cScript_SCEntity* pSCEntity = pChild->AddComponent<cScript_SCEntity>();
		}
	}

	Ptr<cPrefab> pPrefab = new cPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(SC::GetUnitName(SC::eUNIT_ID::VESPENE_GEYSER));
}

void ManualEdit::Map_Prefab_Save()
{
	cResMgr* pResMgr = cResMgr::GetInst();

	cGameObject* pObj = new cGameObject;
	pObj->SetKey(SC::strKey_PREFAB::MAPOBJ);
	pObj->SetLayer(SC::LAYER_INFO::TileMap);

	//MeshRender
	{
		pObj->AddComponent<cCom_Renderer_TilemapSC>();
	}

	Ptr<cPrefab> pPrefab = new cPrefab;
	pPrefab->RegisterPrefab(pObj);
	pPrefab->Save(SC::strKey_PREFAB::MAPOBJ);
}

Ptr<cAnim2DAtlas> ManualEdit::LoadAnim(const string& _strKey)
{
	Ptr<cAnim2DAtlas> pAtlas = cResMgr::GetInst()->Load<cAnim2DAtlas>(_strKey);
	assert(nullptr != pAtlas);

	return pAtlas;
}

Ptr<cPrefab> ManualEdit::LoadPrefab(const string& _strKey)
{
	Ptr<cPrefab> pPrefab = cResMgr::GetInst()->Load<cPrefab>(_strKey);
	assert(nullptr != pPrefab);

	return pPrefab;
}

