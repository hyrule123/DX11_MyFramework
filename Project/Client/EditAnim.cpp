#include "pch.h"

#include <Script/strKeyTexture.h>

#include <Engine/CResMgr.h>


#include "EditAnim.h"


void AnimEditor::EditAnim()
{
	CResMgr* pResMgr = CResMgr::GetInst();
	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(string(RES_TEXTURE::)));

		Atlas->SetNewAnimUV_SC_Redundant(9u, 0u, 9u);
		Atlas->AddAnim2D_SC_Redundant("MOVE", 0u, 9u, 1.f);

		pResMgr->AddRes<CAnim2DAtlas>(string(RES_TEXTURE::), Atlas);
	}

	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
		Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(string(RES_TEXTURE::CORSAIR_BMP)));

		Atlas->SetNewAnimUV(17u, 5u, 0u, 17u, 0u, 5u);
		Atlas->AddAnim2D("MOVE", 0u, 17u, 0u, 5u, 0.3f, eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP);

		pResMgr->AddRes<CAnim2DAtlas>(string(RES_TEXTURE::CORSAIR_BMP), Atlas);
	}


	{
		Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;

		//Atlas->SetAtlasTexture(pResMgr->FindRes<CTexture>(string(RES_TEXTURE::MARINE_BMP)));

		//Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);
		//Atlas->AddAnim2D_SC_Redundant("Marine_Idle", 0u, 1u, 1.f);
		//Atlas->AddAnim2D_SC_Redundant("Marine_AttackStart", 1u, 2u, 0.5f);

		//Atlas->AddAnim2D_SC_Redundant("Marine_Move", 4u, 9u, 1.f);

		//vector<UINT> row = { 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 2u, 2u, 2u, 2u };
		//Atlas->AddAnim2D_vecRowIndex("Marine_Attack", row, 0.6f);

		//row.clear();
		//
		//row = { 13u, 27u, 41u, 55u, 69u, 84u, 98u, 112u };
		//Atlas->AddAnim2D("Marine_Death", row, 1.f);
		//
		//pResMgr->AddRes<CAnim2DAtlas>(string(RES_TEXTURE::MARINE_BMP), Atlas);

		//Atlas->Save("MarineMain.json");

		if (true == Atlas->Load("MarineMain.json"))
		{
			pResMgr->AddRes<CAnim2DAtlas>(Atlas->GetKey(), Atlas);
		}
	}
}

