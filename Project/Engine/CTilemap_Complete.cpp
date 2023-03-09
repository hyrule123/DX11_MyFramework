#include "pch.h"
#include "CTilemap_Complete.h"

#include "CResMgr.h"
#include "strKeys.h"
#include "CCS_SCMapLoader.h"

CTilemap_Complete::CTilemap_Complete()
	: CTilemap(eTILE_TYPE::COMPLETE)
	, m_tMapData()
{
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(DEFAULT_RES::MATERIAL::TILEMAP_COMPLETE);
	SetMaterial(pMtrl);
}

CTilemap_Complete::~CTilemap_Complete()
{
}



bool CTilemap_Complete::render()
{
	if (false == m_bMapLoaded)
		return true;




	return true;
}

bool CTilemap_Complete::LoadMap(const wstring& _strMapName)
{
	static Ptr<CCS_SCMapLoader> pLoader = CResMgr::GetInst()->FindRes<CComputeShader>(DEFAULT_RES::SHADER::COMPUTE::SC_MAP_LOADER);

	m_bMapLoaded = pLoader->LoadMap(_strMapName, m_tMapData);

	return m_bMapLoaded;
}
