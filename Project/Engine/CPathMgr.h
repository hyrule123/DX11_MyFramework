#pragma once

#include "global.h"

#include "Singleton.h"

#include "strKeyDefault.h"





class CPathMgr
{
	DECLARE_SINGLETON(CPathMgr);
private:
	std::filesystem::path m_PathAbsolute_Content;
	std::filesystem::path m_PathRelative_Content;
	std::filesystem::path m_PathRelative_Resource[(int)eRES_TYPE::END];

	

public:
	void init();
	
public:
	const std::filesystem::path& GetPathAbs_Content() const { return m_PathAbsolute_Content; }
	const std::filesystem::path& GetPathRel_Content() const { return m_PathRelative_Content; }
	const std::filesystem::path& GetPathRel_Resource(eRES_TYPE _eResType) const { return m_PathRelative_Resource[(int)_eResType]; }
};

