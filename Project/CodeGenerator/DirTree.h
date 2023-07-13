#pragma once

//트리 관리자 클래스
#include "CDirTreeNode.h"
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	//시스템 탐색
	HRESULT SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex);

	//파일 명을 헤더 파일에 등록(편의용도)
	HRESULT CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension);

	//UserClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수

	HRESULT CreateCShaderCode(stdfs::path const& _FilePath);

	HRESULT CreateGShaderStrKey(stdfs::path const& _FilePath);

	HRESULT CreateScriptCPP(stdfs::path const& _FilePath);


private:
	CDirTreeNode m_RootDir;

private:
	HRESULT DetectNewShaderGroup(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup);
};











