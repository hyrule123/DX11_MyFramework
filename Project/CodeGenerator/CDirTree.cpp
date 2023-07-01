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
	SAFE_DELETE(m_RootDir);
	m_RootDir = new CDirTreeNode;

	//필터 확장자를 소문자로 변환
	vector<stdfs::path> vecExtLower;
	for (size_t i = 0; i < _vecExtFilter.size(); ++i)
	{
		vecExtLower.push_back(MacroFunc::LowerCase<char>(_vecExtFilter[i].string()));
	}

	return m_RootDir->InitRecursive(_RootPath, vecExtLower);
}

HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	//해당 경로가 존재하지 않을 경우 에러 반환
	if (nullptr == m_RootDir || false == m_RootDir->IsReady())
	{
		DEBUG_BREAK;
		return E_NOT_SET;
	}

	if (false == stdfs::exists(_DirPath))
	{
		DEBUG_BREAK;
		return CO_E_BAD_PATH;
	}

	stdfs::path FullPath = _DirPath / _FileName;
	CCodeWriter<char> Writer;
	Writer.Open(FullPath);

	if (false == Writer.IsOpen())
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

	m_RootDir->WriteStrKey(Writer);

	return S_OK;
}
