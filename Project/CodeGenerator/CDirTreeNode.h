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

private:
	stdfs::path	  m_DirName;
	vector<stdfs::path>	  m_vecFileName;
};



template <typename T>
HRESULT CDirTreeNode::WriteStrKey(CCodeWriter<T>& _pCodeWriter)
{
	CCodeWriter<T>& writer = _pCodeWriter;

	if (IsRoot())
	{
		writer.WriteCode(CONV_PRESET_STR(T, define_Preset::Keyword::Head));

		writer.WriteCode(CONV_STRING(T, "//Base Path: "));
		
		std::basic_string<T> strCode(CONV_STRING(T, "//"));
		strCode += m_DirName.string<T>();

		writer.WriteCode(strCode);

		//루트 노드의 경우 이름도 알맞게 바꿔줘야 함
		strCode.clear();

		writer.WriteCode();
		writer.WriteCode();

		strCode = std::basic_string<T>(CONV_STRING(T, "namespace strKey_"));
	}

	stdfs::path pt;

	writer.WriteCode(CONV_PRESET_STR(define_Preset::))


		return E_NOTIMPL;
}
