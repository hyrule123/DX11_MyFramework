#pragma once

#include "pch.h"
#include "CCodeWriter.h"
#include "MacroFunc.h"


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
	HRESULT SearchRecursive(stdfs::path const& _path, std::regex const& _regex);

	HRESULT GetAllFiles(__out std::vector<stdfs::path>& _vecFile, bool _bAddRelativeDir);

	HRESULT WriteStrKeyTree(CCodeWriter& _CodeWriter, bool _bEraseExtension);

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











