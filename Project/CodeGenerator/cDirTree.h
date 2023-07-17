#pragma once
//트리 관리자 클래스
#include "cDirTreeNode.h"

struct tAddBaseClassDesc
{
	stdfs::path _FilePath;
	std::string IncludeBaseHeaderName;
	std::string IncludeStrKeyHeaderName;
	std::string Constructor_T_MacroDefine;
	std::string UserClassMgr_InitFuncName;
};

class cDirTree
{
public:
	cDirTree();
	~cDirTree();

	//시스템 탐색
	HRESULT SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex);

	//파일 명을 헤더 파일에 등록(편의용도)
	HRESULT CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension);

	//UserClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	HRESULT CreateUserClassInitCode(tAddBaseClassDesc const& _Desc);

	HRESULT CreateShaderStrKey(stdfs::path const& _FilePath);



private:
	cDirTreeNode m_RootDir;

private:
	HRESULT DetectNewGraphicsShader(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup);
	HRESULT DetectNewComputeShader(std::vector<stdfs::path> const& _vecCShader);
};











