#pragma once

#include "pch.h"
#include "CCodeWriter.h"
#include "MacroFunc.h"

class CDirTreeNode
{
	friend class CDirTree;
	friend class CDirTreeNode;
private:
	CDirTreeNode();
	~CDirTreeNode();

public:
	HRESULT InitRecursive(stdfs::path const& _path,std::vector<stdfs::path> const& _vecExtensionFilter);

	template <typename T>
	HRESULT WriteStrKey(CCodeWriter<T>& _pCodeWriter);

private:
	CDirTreeNode* m_pParent;
	std::vector<CDirTreeNode*> m_vecChild;

public:
	void SetParent(CDirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(CDirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); _pNode->SetParent(_pNode); }

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
HRESULT CDirTreeNode::WriteStrKey(CCodeWriter<T>& _pCodeWriter)
{

	CCodeWriter<T>& writer = _pCodeWriter;

	if (IsRoot())
	{
		writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));

		writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

		writer.WriteCode(T_STRING(T, "//Base Path: "));
		
		std::basic_string<T> strCode(T_STRING(T, "//"));
		strCode += m_DirName.string<T>();

		writer.WriteCode(strCode);

		//루트 노드의 경우 이름도 알맞게 바꿔줘야 함
		strCode.clear();

		writer.WriteCode();
		writer.WriteCode();

		strCode = NEW_T_STRING(T, "namespace strKey_");

		strCode += MacroFunc::UpperCase<T>(m_DirName.filename().string<T>());

		writer.WriteCode(strCode);
	}
	else
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
			strCode += MacroFunc::ConvertToVarName<T>(m_vecFileName[i].string<T>());
			strCode += T_STRING(T, " = \"");
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
			HRESULT hr = m_vecChild[i]->WriteStrKey(_pCodeWriter);
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

