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

HRESULT CDirTreeNode::SearchRecursive(stdfs::path const& _path, tDirTreeFilters const& _Filter)
{
	//들어온 Path 자체가 폴더 경로가 아닐 경우에는 실패 반환
	if (false == stdfs::is_directory(_path))
		return E_INVALIDARG;

	//디렉토리 이름을 등록
	if (IsRoot())
		m_DirName = _path;
	else
		m_DirName = _path.filename();

	try
	{
		for (const auto& dirIter : stdfs::directory_iterator(_path))
		{
			//대소문자 가리지 않고 비교를 위해 소문자로 변경
			const wstring& LowerFileName = MacroFunc::LowerCase<wchar_t>(dirIter.path().filename().wstring());

			//제외 항목 검사
			bool bExclude = false;
			for (size_t i = 0; i < _Filter.vecExcludeKeyword.size(); ++i)
			{
				if (std::wstring::npos != LowerFileName.find(_Filter.vecExcludeKeyword[i]))
				{
					bExclude = true;
					break;
				}
			}
			if (bExclude)
				continue;


			//포함 항목 검사
			
			//파일명일 경우 - 확장자 및 파일명을 확인하고, 일치하는 경우에만 파일명을 등록
			if (false == dirIter.is_directory())
			{
				bool bPushFileName = false;
				if (_Filter.vecIncludeKeyword.empty())
					bPushFileName = true;
				else
				{
					for (size_t i = 0; i < _Filter.vecIncludeKeyword.size(); ++i)
					{
						if (std::wstring::npos != LowerFileName.find(_Filter.vecIncludeKeyword[i]))
						{
							bPushFileName = true;
							break;
						}
					}
				}

				//마지막으로 확장자가 일치하는지 확인하고 파일 삽입
				if (bPushFileName)
				{
					for (size_t i = 0; i < _Filter.vecExtInclude.size(); ++i)
					{
						//확장자의 경우 정확히 일치하는지 확인
						if (_Filter.vecExtInclude[i].wstring() == MacroFunc::LowerCase<wchar_t>(dirIter.path().filename().extension().wstring()))
						{
							m_vecFileName.push_back(dirIter.path().filename());
							break;
						}
					}
				}
			}

			//폴더명일 경우
			else
			{
				//폴더를 발견했을 경우 새 노드를 생성 후 재귀호출
				CDirTreeNode* pNode = new CDirTreeNode;
				AddChild(pNode);

				HRESULT hr = pNode->SearchRecursive(dirIter.path(), _Filter);

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
