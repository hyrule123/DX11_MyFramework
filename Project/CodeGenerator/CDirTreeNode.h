#pragma once

#include "pch.h"
#include "CCodeWriter.h"
#include "MacroFunc.h"


struct tDirTreeFilters
{
	std::vector<std::filesystem::path> vecExtInclude;
	std::vector<std::filesystem::path> vecIncludeKeyword;
	std::vector<std::filesystem::path> vecExcludeKeyword;
};

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
	~CDirTreeNode();

public:
	HRESULT SearchRecursive(stdfs::path const& _path, tDirTreeFilters const& _Filter);

	template <typename T>
	HRESULT WriteStrKey(CCodeWriter<T>& _CodeWriter, bool _bEraseExtension);

	//쉐이더 파일 전체 확인해서 쉐이더 그룹으로 정리해서 반환
	template <typename T>
	HRESULT WriteComputeShaderCode(CCodeWriter<T>& _CodeWriter);


	HRESULT WriteGraphicsShaderStrKey(std::unordered_map<stdfs::path, tShaderGroup>& _umapShaderGroup);


private:
	CDirTreeNode* m_pParent;
	std::vector<CDirTreeNode*> m_vecChild;

public:
	void SetParent(CDirTreeNode* _pNode) { assert(_pNode); m_pParent = _pNode; }
	void AddChild(CDirTreeNode* _pNode) { assert(_pNode); m_vecChild.push_back(_pNode); _pNode->SetParent(_pNode); }

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


template <typename T>
HRESULT CDirTreeNode::WriteStrKey(CCodeWriter<T>& _pCodeWriter, bool _bEraseExtension)
{
	CCodeWriter<T>& writer = _pCodeWriter;

	
	if (IsRoot())
	{
		std::basic_string<T> strCode = NEW_T_STRING(T, "namespace strKey_");
		strCode += MacroFunc::UpperCase<T>(m_DirName.filename().string<T>());
		writer.WriteCode(strCode);
	}
	else
	{
		std::basic_string<T> strCode = NEW_T_STRING(T, "namespace ");
		strCode += MacroFunc::UpperCase<T>(m_DirName.filename().string<T>());
		writer.WriteCode(strCode);
	}


	//중괄호 열고 자신의 파일목록 작성
	{
		writer.OpenBracket();

		size_t size = m_vecFileName.size();
		for (size_t i = 0; i < size; ++i)
		{
			std::basic_string<T> strCode(T_STRING(T, "STRKEY "));
			strCode += MacroFunc::ConvertToVarName<T>(m_vecFileName[i].string<T>());
			strCode += T_STRING(T, " = \"");

			if (false == IsRoot())
			{
				strCode += m_DirName.string<T>();
				strCode += "/";
			}

			if (_bEraseExtension)
				strCode += m_vecFileName[i].filename().replace_extension("").string<T>();
			else
				strCode += m_vecFileName[i].string<T>();
				

			strCode += T_STRING(T, "\";");
			writer.WriteCode(strCode);
		}
	}

	//자식 노드가 있을 경우 재귀 호출 
	{
		size_t size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			HRESULT hr = m_vecChild[i]->WriteStrKey(_pCodeWriter, _bEraseExtension);
			if (FAILED(hr))
			{
				return E_FAIL;
			}
		}
	}

	writer.CloseBracket();
	writer.WriteCode();

	return S_OK;
}



template<typename T>
inline HRESULT CDirTreeNode::WriteComputeShaderCode(CCodeWriter<T>& _CodeWriter)
{
	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for(size_t i = 0; i < m_vecFileName.size(); ++i)
	{
		//0번 버퍼에 include 작성
		{
			const std::basic_string<T>& FileName = m_vecFileName[i].filename().string<T>();

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin);
			strCode += FileName;
			strCode += T_STRING(T, "\"");
			_CodeWriter.WriteCode(strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::basic_string<T>& ClassName = m_vecFileName[i].filename().replace_extension("").string<T>();

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::T_Constructor);
			strCode += ClassName;
			strCode += T_STRING(T, ");");
			_CodeWriter.WriteCode(strCode, 1);
		}
	}

	return S_OK;
}

inline HRESULT CDirTreeNode::WriteGraphicsShaderStrKey(std::unordered_map<stdfs::path, tShaderGroup>& _umapShaderGroup)
{
	//CCodeWriter<T>& writer = _CodeWriter;

	for (size_t i = 0; i < m_vecFileName.size(); ++i)
	{
		//string으로 변경
		string strFileName = m_vecFileName[i].string();

		for (int j = 0; j < (int)define_Shader::eGS_TYPE::END; ++j)
		{
			size_t Pos = strFileName.find(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]);
			if (string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				strFileName.erase(0, Pos + strlen(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]));

				//파일의 확장자를 json으로 변경하고, 키값으로 사용.
				stdfs::path ShaderGroupName = strFileName;
				ShaderGroupName.replace_extension(RES_INFO::SHADER::Ext_ShaderSetting);

				//쉐이더 그룹명에 각 쉐이더 파이프라인의 데이터를 담은 파일의 이름을 추가한다.
				_umapShaderGroup[ShaderGroupName].FileName[j] = m_vecFileName[i];

				break;
			}
		}
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->WriteGraphicsShaderStrKey(_umapShaderGroup);
	}

	return S_OK;
}







