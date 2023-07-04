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

HRESULT CDirTree::SearchRecursive(stdfs::path const& _RootPath, vector<stdfs::path>& _vecExtFilter, vector<stdfs::path>& _vecExcludeDir)
{
	m_RootDir.Clear();

	MacroFunc::ConvertPathToLowerCase(_vecExtFilter);
	MacroFunc::ConvertPathToLowerCase(_vecExcludeDir);

	return m_RootDir.SearchRecursive( _RootPath, _vecExtFilter, _vecExcludeDir);
}



