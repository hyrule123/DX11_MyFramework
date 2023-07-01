#include "pch.h"

#include <Engine/define.h>

#include "CDirTreeNode.h"
#include "MacroFunc.h"
#include "define_PresetString.h"
#include "CCodeWriter.h"

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

HRESULT CDirTreeNode::InitRecursive(stdfs::path const& _path, std::vector<stdfs::path> const& _vecExtensionFilter)
{
	//들어온 Path 자체가 폴더 경로가 아닐 경우에는 실패 반환
	if (false == stdfs::is_directory(_path))
		return E_INVALIDARG;

	if (IsRoot())
		m_DirName = _path;
	else
		m_DirName = _path.filename();

	try
	{
		for (const auto& dirIter : stdfs::directory_iterator(_path))
		{
			//폴더가 아닐 경우 확장자를 확인하고, 일치하는 경우에만 파일명을 등록
			if (false == dirIter.is_directory())
			{
				if(_vecExtensionFilter.empty())
					m_vecFileName.push_back(dirIter.path().filename());
				else
				{
					//확장자를 소문자로 바꿔서 비교(인자로 들어온 확장자 filter의 경우 CDirTree에서 변환했음)
					const stdfs::path& fileName = dirIter.path().filename();
					const string& lowerExt = MacroFunc::LowerCase(fileName.extension().string());
						
					for (size_t i = 0; i < _vecExtensionFilter.size(); ++i)
					{
						if (lowerExt == _vecExtensionFilter[i])
						{
							m_vecFileName.push_back(fileName);
							continue;
						}
					}
				}
			}

			else
			{
				//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
				CDirTreeNode* pNode = new CDirTreeNode;
				AddChild(pNode);

				HRESULT hr = pNode->InitRecursive(dirIter.path(), _vecExtensionFilter);

				if (FAILED(hr))
				{
					SAFE_DELETE(pNode);
					return E_FAIL;
				}
				
			}
		}
	}
	catch (stdfs::filesystem_error const& _err)
	{
		ERROR_MESSAGE(_err.what());
		return E_ACCESSDENIED;
	}

	return S_OK;
}
