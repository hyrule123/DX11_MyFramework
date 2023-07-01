#pragma once

class CDirTreeNode;
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	HRESULT InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter);
	HRESULT CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName);

private:
	CDirTreeNode* m_pRootDir;
};

