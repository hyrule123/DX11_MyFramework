#pragma once

#include "pch.h"
#include "CCodeWriter.h"
#include "MacroFunc.h"


struct tDirTreeFilters
{
	std::vector<std::filesystem::path> vecExtInclude;
	std::vector<std::filesystem::path> vecIncludeKeyword;
	std::vector<std::filesystem::path> vecExcludeKeyword;
};

struct tShaderGroup
{
	stdfs::path FileName[(int)define_Shader::eGS_TYPE::END];
};

class CDirTreeNode
{
	friend class CDirTree;
	friend class CDirTreeNode;
private:
	CDirTreeNode();
	CDirTreeNode(CDirTreeNode* _pParent);
	~CDirTreeNode();

public:
	HRESULT SearchRecursive(stdfs::path const& _path, tDirTreeFilters const& _Filter);

	HRESULT GetAllFiles(__out std::vector<stdfs::path>& _vecFile, bool _bAddRelativeDir);


	template <typename T>
	HRESULT WriteStrKeyTree(CCodeWriter<T>& _CodeWriter, bool _bEraseExtension, stdfs::path _RootNamespaceName = stdfs::path());

private:
	CDirTreeNode* m_pParent;
	std::vector<CDirTreeNode*> m_vecChild;

public:
	void SetParent(CDirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(CDirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); }

	bool IsRoot() const { return (nullptr == m_pParent); }
	bool IsLeaf() const { return m_vecChild.empty(); }
	bool IsReady() const { return !m_DirName.empty(); }

private:
	stdfs::path	  m_DirName;
	vector<stdfs::path>	  m_vecFileName;

	//DirTree에서 사용
private:
	void Clear();
};


template <typename T>
HRESULT CDirTreeNode::WriteStrKeyTree(CCodeWriter<T>& _pCodeWriter, bool _bEraseExtension, stdfs::path _RootNamespaceName)
{
	CCodeWriter<T>& writer = _pCodeWriter;

	if (false == IsRoot())
	{
		std::basic_string<T> strCode = NEW_T_STRING(T, "namespace ");
		strCode += MacroFunc::UpperCase<T>(m_DirName.filename().string<T>());
		writer.WriteCode(strCode);
	}

	//중괄호 열고 자신의 파일목록 작성
	{
		writer.OpenBracket();

		size_t size = m_vecFileName.size();
		for (size_t i = 0; i < size; ++i)
		{
			std::basic_string<T> strCode(T_STRING(T, "STRKEY "));

			if (_bEraseExtension)
				strCode += MacroFunc::ConvertToVarName<T>(m_vecFileName[i].filename().replace_extension("").string<T>());
			else
				strCode += MacroFunc::ConvertToVarName<T>(m_vecFileName[i].filename().string<T>());

			
			strCode += T_STRING(T, " = \"");

			if (false == IsRoot())
			{
				strCode += m_DirName.string<T>();
				strCode += "/";
			}

			if (_bEraseExtension)
				strCode += m_vecFileName[i].filename().replace_extension("").string<T>();
			else
				strCode += m_vecFileName[i].string<T>();
				

			strCode += T_STRING(T, "\";");
			writer.WriteCode(strCode);
		}
	}

	//자식 노드가 있을 경우 재귀 호출 
	{
		size_t size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			HRESULT hr = m_vecChild[i]->WriteStrKeyTree(_pCodeWriter, _bEraseExtension);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
	}

	writer.CloseBracket();
	writer.WriteCode();

	return S_OK;
}








