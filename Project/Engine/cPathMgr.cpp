#include "pch.h"
#include "cPathMgr.h"

#include "cEngine.h"


cPathMgr::cPathMgr()
{
}

cPathMgr::~cPathMgr()
{
}


void cPathMgr::Init()
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

