#pragma once

#include "pch.h"
#include "CCodeWriter.h"
#include "MacroFunc.h"


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
	HRESULT SearchRecursive(stdfs::path const& _path, std::vector<stdfs::path> const& _vecExtensionFilter, std::vector<stdfs::path> const& _vecFolderExclude = std::vector<stdfs::path>());

	template <typename T>
	HRESULT WriteStrKey(CCodeWriter<T>& _CodeWriter);


	//쉐이더 파일 전체 확인해서 쉐이더 그룹으로 정리해서 반환
	template <typename T>
	HRESULT WriteShaderCode(__out std::unordered_map<stdfs::path, tShaderGroup>& _umapShaderSetting, __out std::vector<stdfs::path> _vecComputeShader);


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
HRESULT CDirTreeNode::WriteStrKey(CCodeWriter<T>& _pCodeWriter)
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
			HRESULT hr = m_vecChild[i]->WriteStrKey(_pCodeWriter);
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
inline HRESULT CDirTreeNode::WriteShaderCode(__out std::unordered_map<stdfs::path, tShaderGroup>& _umapShaderSetting, __out std::vector<stdfs::path> _vecComputeShader)
{
	//CCodeWriter<T>& writer = _CodeWriter;

	size_t size = m_vecFileName.size();
	for (size_t i = 0; i < size; ++i)
	{
		//새로 복사를 하나 받아서
		stdfs::path FileNamePath = m_vecFileName[i];

		//확장자를 json으로 바꿔주고 string으로 변경
		string ShaderGroupName = FileNamePath.replace_extension(RES_INFO::SHADER::Ext_ShaderSetting).string();

		bool bFoundGS = false;
		static const int GSEnd = (int)define_Shader::eGS_TYPE::END;
		for (int j = 0; j < size; ++j)
		{
			size_t Pos = ShaderGroupName.find(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]);
			if (string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				ShaderGroupName.erase(0, Pos + strlen(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]));

				//쉐이더 그룹명에 각 쉐이더 파이프라인의 데이터를 담은 파일의 이름을 추가한다.
				_umapShaderSetting[ShaderGroupName].FileName[j] = m_vecFileName[i];

				bFoundGS = true;

				break;
			}
		}

		//그래픽 쉐이더가 아닐 경우 컴퓨트 쉐이더인지 확인
		if (false == bFoundGS)
		{
			size_t Pos = ShaderGroupName.find(RES_INFO::SHADER::COMPUTE::Prefix);
			if (string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				ShaderGroupName.erase(0, Pos + (size_t)1);

				_vecComputeShader.push_back(ShaderGroupName);
			}
		}
	}


	return S_OK;
}

