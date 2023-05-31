#include "pch.h"
#include "ManualEdit.h"

#include <Script/strKeyTexture.h>

#include <Engine/CResMgr.h>
#include <Script/define_SCUnit.h>

#include <Engine/CScriptHolder.h>

void ManualEdit::EditPrefab()
{
	MarineAnim();
	MarinePrefab();


}


void ManualEdit::MarineAnim()
{
	CResMgr* pResMgr = CResMgr::GetInst();

	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

	Atlas->SetAtlasTexture(CResMgr::GetInst()->FindRes<CTexture>(string(strKey_Texture::TERRAN::MARINE_BMP)));

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

	string strKey = string(strKey_Texture::TERRAN::MARINE_BMP);
	CResMgr::GetInst()->AddRes<CAnim2DAtlas>(strKey, Atlas);
	Atlas->Save(strKey);
}

void ManualEdit::MarinePrefab()
{
	if

	{



	}
}

void ManualEdit::LoadAnim()
{
	if (true == Atlas->Load("MarineMain.json"))
	{
		pResMgr->AddRes<CAnim2DAtlas>(Atlas->GetKey(), Atlas);
	}
}


//CResMgr* pResMgr = CResMgr::GetInst();
//Reaver
//{
//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
//	Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(string(strKey_Texture::)));

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
