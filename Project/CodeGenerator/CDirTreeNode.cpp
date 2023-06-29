#include "pch.h"

#include <Engine/define.h>

#include "CDirTreeNode.h"


CDirTreeNode::CDirTreeNode()
	: m_pParent()
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
}

CDirTreeNode::~CDirTreeNode()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

HRESULT CDirTreeNode::InitRecursive(stdfs::path const& _path, vector<stdfs::path> const& _vecExtensionFilter)
{
	if (false == stdfs::is_directory(_path))
		return E_FAIL;

	try
	{
		for (const auto& dirIter : stdfs::directory_iterator(_path))
		{
			//폴더가 아닐 경우 파일명을 vector에 등록
			if (false == dirIter.is_directory())
			{
				m_vecFileName.push_back(dirIter.path().filename());
			}

			//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
			else
			{
				CDirTreeNode* pNode = new CDirTreeNode;

				HRESULT hr = InitRecursive(dirIter.path(), _vecExtensionFilter);

				if (FAILED(hr))
				{
					SAFE_DELETE(pNode);
					return E_FAIL;
				}

				AddChild(pNode);
			}
		}
	}
	catch (stdfs::filesystem_error const& _err)
	{
		ERROR_MESSAGE(_err.what());
		return E_FAIL;
	}

	return S_OK;
}
