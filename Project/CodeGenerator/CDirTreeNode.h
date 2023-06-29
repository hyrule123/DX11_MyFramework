#pragma once

#include "pch.h"
#include "CDirTree.h"

class CDirTreeNode
{
	friend class CDirTree;
	friend class CDirTreeNode;
private:
	CDirTreeNode();
	~CDirTreeNode();

public:
	HRESULT InitRecursive(stdfs::path const& _path, vector<stdfs::path> const& _vecExtensionFilter);

private:
	CDirTreeNode* m_pParent;
	std::vector<CDirTreeNode*> m_vecChild;

public:
	void SetParent(CDirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(CDirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); _pNode->SetParent(_pNode); }

private:

	stdfs::path	  m_DirName;
	vector<stdfs::path>	  m_vecFileName;

public:

};

