#include "pch.h"
#include "CDirTree.h"

#include "CDirTreeNode.h"

CDirTree::CDirTree()
	: m_pRootDir()
{
}

CDirTree::~CDirTree()
{
}

HRESULT CDirTree::InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter)
{
	m_pRootDir = make_unique<CDirTreeNode>();

	return m_pRootDir->InitRecursive(_RootPath, _vecExtFilter);
}
