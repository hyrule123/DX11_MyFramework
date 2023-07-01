#include "pch.h"
#include "CDirTree.h"

#include "CDirTreeNode.h"
#include "MacroFunc.h"



CDirTree::CDirTree()
	: m_pRootDir()
{
}

CDirTree::~CDirTree()
{
	SAFE_DELETE(m_pRootDir);
}

HRESULT CDirTree::InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter)
{
	SAFE_DELETE(m_pRootDir);

	m_pRootDir = new CDirTreeNode;

	//필터 확장자를 소문자로 변환
	vector<stdfs::path> vecExtLower;
	for (size_t i = 0; i < _vecExtFilter.size(); ++i)
	{
		vecExtLower.push_back(MacroFunc::LowerCase(_vecExtFilter[i].string()));
	}

	return m_pRootDir->InitRecursive(_RootPath, vecExtLower);
}

HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	//해당 경로가 존재하지 않을 경우 에러 반환
	if (false == stdfs::exists(_DirPath))
	{
		DEBUG_BREAK;
		return CO_E_BAD_PATH;
	}

	stdfs::path FullPath = _DirPath / _FileName;
	std::ofstream ofs(FullPath);
	if (false == ofs.is_open())
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

	return S_OK;
}
