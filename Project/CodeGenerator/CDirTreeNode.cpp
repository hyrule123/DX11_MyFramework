#include "pch.h"

#include <Engine/define.h>

#include "CDirTreeNode.h"
#include "MacroFunc.h"
#include "define_CodeGen.h"
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

void CDirTreeNode::Clear()
{
	size_t size = m_vecChild.size();
	for (size_t i = (size_t)0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
	m_vecChild.clear();
	m_DirName.clear();
	m_pParent = nullptr;
}

HRESULT CDirTreeNode::SearchRecursive(stdfs::path const& _path, std::vector<stdfs::path> const& _vecExtensionFilter, std::vector<stdfs::path> const& _vecFolderExclude)
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

				bool bPushFileName = false;
				if (_vecExtensionFilter.empty())
					bPushFileName = true;
				else
				{
					//확장자를 소문자로 바꿔서 비교(인자로 들어온 확장자 filter의 경우 CDirTree에서 변환했음)
					const wstring& lowerExt = MacroFunc::LowerCase<wchar_t>(dirIter.path().filename().extension().wstring());
					for (size_t i = 0; i < _vecExtensionFilter.size(); ++i)
					{
						if (lowerExt == _vecExtensionFilter[i])
						{
							bPushFileName = true;
							break;
						}
					}
				}

				if (bPushFileName)
				{
					m_vecFileName.push_back(dirIter.path().filename());
				}
			}

			else
			{
				//제외 대상 폴더인지 확인
				bool bExclude = false;
				for (size_t i = 0; i < _vecFolderExclude.size(); ++i)
				{
					if (_vecFolderExclude[i] == MacroFunc::LowerCase<wchar_t>(_path.filename().wstring()))
					{
						bExclude = true;
						break;
					}
				}
				if (bExclude)
					continue;

				//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
				CDirTreeNode* pNode = new CDirTreeNode;
				AddChild(pNode);

				HRESULT hr = pNode->SearchRecursive(dirIter.path(), _vecExtensionFilter);

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
