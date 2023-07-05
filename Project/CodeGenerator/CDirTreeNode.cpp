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

CDirTreeNode::CDirTreeNode(CDirTreeNode* _pParent)
	: m_pParent(_pParent)
	, m_vecChild()
	, m_DirName()
	, m_vecFileName()
{
	assert(_pParent);
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
				CDirTreeNode* pNode = new CDirTreeNode(this);
				HRESULT hr = pNode->SearchRecursive(dirIter.path(), _Filter);
				
				if (ERROR_EMPTY == hr)
				{
					SAFE_DELETE(pNode);
					continue;
				}
				else if (FAILED(hr))
				{
					SAFE_DELETE(pNode);
					return hr;
				}


				AddChild(pNode);
			}
		}

		//순회를 다 돌았는데 자식 노드도 없고 자신에게 파일도 안들어 있을경우 -> ERROR_EMPTY 반환
		//ERROR_EMPTY가 반환되면 해당 노드가 제거됨.
		if (IsLeaf() && m_vecFileName.empty())
			return ERROR_EMPTY;
	}
	catch (stdfs::filesystem_error const& _err)
	{
		ERROR_MESSAGE(_err.what());
		return E_ACCESSDENIED;
	}

	return S_OK;
}



HRESULT CDirTreeNode::GetAllFiles(std::vector<stdfs::path>& _vecFile, bool _bAddRelativeDir)
{
	for (size_t i = 0; i < m_vecFileName.size(); ++i)
	{
		if (IsRoot() || _bAddRelativeDir)
		{
			_vecFile.push_back(m_vecFileName[i]);
		}
		else
		{
			_vecFile.push_back(m_DirName / m_vecFileName[i]);
		}
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		HRESULT hr = m_vecChild[i]->GetAllFiles(_vecFile, _bAddRelativeDir);
		if (FAILED(hr))
		{
			DEBUG_BREAK;
			return hr;
		}
	}

	return S_OK;
}
