#pragma once


//트리 관리자 클래스
#include "CDirTreeNode.h"
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	//시스템 탐색
	HRESULT InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter);

	//파일 명을 헤더 파일에 등록(편의용도)
	template <typename T>
	HRESULT CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName);




private:
	CDirTreeNode m_RootDir;

	HRESULT CheckReady(stdfs::path const& _DirPath);
};


template<typename T>
inline HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	//아직 경로가 준비되지 않았을 경우 시작 X
	if (false == m_RootDir.IsReady())
	{
		DEBUG_BREAK;
		return E_NOT_SET;
	}

	//해당 경로가 존재하지 않을 경우에도 에러 리턴
	else if (false == stdfs::exists(_DirPath))
	{
		DEBUG_BREAK;
		return CO_E_BAD_PATH;
	}

	stdfs::path FullPath = _DirPath / _FileName;
	CCodeWriter<T> Writer;
	Writer.Open(FullPath);
	if (false == Writer.IsOpen())
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

	HRESULT hr = m_RootDir.WriteStrKey(Writer);

	if (FAILED(hr))
		return hr;

	hr = Writer.SaveAll();

	return hr;
}



