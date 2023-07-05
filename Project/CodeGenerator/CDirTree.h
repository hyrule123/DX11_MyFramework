#pragma once

//트리 관리자 클래스
#include "CDirTreeNode.h"
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	//시스템 탐색
	HRESULT SearchRecursive(stdfs::path const& _RootPath, tDirTreeFilters const& _Filter);

	//파일 명을 헤더 파일에 등록(편의용도)
	template <typename T>
	HRESULT CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName, bool _bEraseExtension);

	//UserClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	template <typename T>
	HRESULT CreateComputerShaderCode(stdfs::path const& _DirPath, stdfs::path const& _FileName);

	template <typename T>
	HRESULT CreateGraphicsShaderStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName);

	template <typename T>
	HRESULT CreateScriptCode(stdfs::path const& _DirPath, stdfs::path const& _FileName);


private:
	CDirTreeNode m_RootDir;

private:
	template <typename T>
	HRESULT OpenWriter(stdfs::path const& _DirPath, stdfs::path const& _FileName, CCodeWriter<T>& _Writer);

	HRESULT DetectNewShaderGroup(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup);
};


#include "CDirTree.inl"
