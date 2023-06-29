#pragma once

class CDirTreeNode;
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	HRESULT InitRecursive(stdfs::path const& _RootPath, vector<stdfs::path> const& _vecExtFilter);

private:
	
	std::unique_ptr<CDirTreeNode> m_pRootDir;
};

