#pragma once


//트리 관리자 클래스
#include "CDirTreeNode.h"
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	//시스템 탐색
	HRESULT SearchRecursive(stdfs::path const& _RootPath, vector<stdfs::path>& _vecExtFilter, vector<stdfs::path>& _vecExcludeDir);

	//파일 명을 헤더 파일에 등록(편의용도)
	template <typename T>
	HRESULT CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName);

	//UserClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	template <typename T>
	HRESULT CreateShaderClassCode(stdfs::path const& _DirPath, stdfs::path const& _FileName);


private:
	CDirTreeNode m_RootDir;

	template <typename T>
	HRESULT OpenWriter(stdfs::path const& _DirPath, stdfs::path const& _FileName, CCodeWriter<T>& _Writer);
};


template<typename T>
inline HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
		return hr;

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode();
	Writer.WriteCode();

	hr = m_RootDir.WriteStrKey(Writer);

	if (FAILED(hr))
		return hr;

	hr = Writer.SaveAll();

	return hr;
}



template<typename T>
inline HRESULT CDirTree::CreateShaderClassCode(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin));

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode();
	Writer.WriteCode();


	std::unordered_map<stdfs::path, tShaderGroup> umapGS;
	std::vector<stdfs::path> vecCompute;

	m_RootDir.WriteShaderCode<T>(umapGS, vecCompute);

		
	//hr = m_RootDir.WriteShaderCode<T>(Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}
		
	hr = Writer.SaveAll();
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;
}


template<typename T>
inline HRESULT CDirTree::OpenWriter(stdfs::path const& _DirPath, stdfs::path const& _FileName, CCodeWriter<T>& _Writer)
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

	_Writer.Open(FullPath);
	if (false == _Writer.IsOpen())
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

	return S_OK;
}
