#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

DEFINITION_SINGLETON(CPathMgr);

CPathMgr::CPathMgr()
{
}

CPathMgr::~CPathMgr()
{
}


void CPathMgr::init()
{
	m_PathAbsolute_Content = std::filesystem::current_path();

	m_PathRelative_Content = std::filesystem::path(DIRECTORY_NAME::RELATIVE_BEGIN);
	m_PathRelative_Content /= DIRECTORY_NAME::CONTENT;


	for (int i = 0; i < (int)eRES_TYPE::END; ++i)
	{
		m_PathRelative_Resource[i] = m_PathRelative_Content;
		m_PathRelative_Resource[i] /= DIRECTORY_NAME::RES_ARR[i];
	}
}

