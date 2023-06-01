#include "pch.h"
#include "ManualEdit.h"

#include <Engine/CResMgr.h>

//string Keys
#include <Engine/strKey_Default.h>
#include <Script/strKey_Texture.h>

//Components
#include <Engine/CCollider2D_Rect.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CScriptHolder.h>

//Scripts
#include <Script/define_SCUnit.h>

void ManualEdit::Edit()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	//마린
	{
		string strKey = strKey_TEXTURE::TERRAN::MARINE_BMP;
		MarineAnim_Save(strKey);
		LoadAnim(strKey);
	}
	
}


void ManualEdit::MarineAnim_Save(const string& _strKey)
{
	CResMgr* pResMgr = CResMgr::GetInst();

	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

	Atlas->SetAtlasTexture(CResMgr::GetInst()->FindRes<CTexture>(strKey_TEXTURE::TERRAN::MARINE_BMP));

	using namespace FSM_SCUnit;
	Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);
	Atlas->AddAnim2D_SC_Redundant(string(strKey_Anim[IDLE]), 0u, 1u, 1.f);
	Atlas->AddAnim2D_SC_Redundant(string(strKey_Anim[ATTACK_BEGIN]), 1u, 2u, 0.5f);
	Atlas->AddAnim2D_SC_Redundant(string(strKey_Anim[ATTACK_END]), 2u, 1u, 0.5f);

	Atlas->AddAnim2D_SC_Redundant(string(strKey_Anim[MOVE]), 4u, 9u, 1.f);

	vector<UINT> row = { 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 2u, 2u, 2u, 2u };
	Atlas->AddAnim2D_vecRowIndex(string(strKey_Anim[ATTACK]), row, 0.6f);

	row.clear();

	row = { 13u, 27u, 41u, 55u, 69u, 84u, 98u, 112u };
	Atlas->AddAnim2D(string(strKey_Anim[DEATH]), row, 1.f);

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
		CMeshRender* pMesh = new CMeshRender;
		pObj->AddComponent(pMesh);

		//CMaterial* pMtrl = pResMgr->FindRes<CMaterial>()
		//pMesh->SetMaterial()
	}
	

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetKey(_strKey);
	pPrefab->RegisterPrefab(pObj, true);
}

void ManualEdit::LoadAnim(const string& _strKey)
{
	Ptr<CAnim2DAtlas> pAtlas = CResMgr::GetInst()->Load<CAnim2DAtlas>(_strKey);
	
	assert(nullptr != pAtlas);
}

void ManualEdit::LoadPrefab(const string& _strKey)
{
	
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
