#pragma once

#include "pch.h"
#include "cCodeWriter.h"
#include <Engine/func.h>


struct tShaderGroup
{
	stdfs::path FileName[(int)define_Shader::eGS_TYPE::END];
};


class cDirTreeNode
{
	friend class cDirTree;
	friend class cDirTreeNode;
private:
	cDirTreeNode();
	cDirTreeNode(cDirTreeNode* _pParent);
	~cDirTreeNode();

public:
	HRESULT SearchRecursive(stdfs::path const& _rootPath, stdfs::path const& _path, std::regex const& _regex);

	HRESULT GetAllFiles(__out std::vector<stdfs::path>& _vecFile, bool _bAddRelativeDir);

	HRESULT WriteStrKeyTree(cCodeWriter& _CodeWriter, bool _bEraseExtension);

private:
	cDirTreeNode* m_pParent;
	std::vector<cDirTreeNode*> m_vecChild;

public:
	void SetParent(cDirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(cDirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); }

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











