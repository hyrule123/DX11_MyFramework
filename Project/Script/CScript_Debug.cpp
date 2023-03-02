#include "pch.h"
#include "CScript_Debug.h"

#include <Engine/CCS_SCMapLoader.h>

CScript_Debug::CScript_Debug()
	: CScript(TYPE_INDEX(CScript_Debug))
{
}

CScript_Debug::~CScript_Debug()
{
}


void CScript_Debug::init()
{
}

void CScript_Debug::tick()
{
	if (nullptr != m_pCS)
		m_pCS->Execute();
}

void CScript_Debug::SetCS(Ptr<CCS_SCMapLoader> _pCS)
{
	m_pCS = _pCS;
}
