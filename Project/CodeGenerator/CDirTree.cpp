#include "pch.h"
#include "CDirTree.h"

#include "MacroFunc.h"

#include "CCodeWriter.h"
#include "CDirTreeNode.h"

CDirTree::CDirTree()
	: m_RootDir()
{
}

CDirTree::~CDirTree()
{
}

HRESULT CDirTree::InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter)
{
	m_RootDir.Clear();

	//필터 확장자를 소문자로 변환
	vector<stdfs::path> vecExtLower;
	for (size_t i = 0; i < _vecExtFilter.size(); ++i)
	{
		vecExtLower.push_back(MacroFunc::LowerCase<char>(_vecExtFilter[i].string()));
	}

	return m_RootDir.InitRecursive(_RootPath, vecExtLower);
}



