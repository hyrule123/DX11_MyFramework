#pragma once

#include "define_CodeGen.h"
#include <Engine/func.h>


class cCodeWriter
{
public:
	cCodeWriter();
	~cCodeWriter();

private:
	struct CodeBuffer
	{
		int iIndentation;
		std::string Buffer;

		CodeBuffer() : iIndentation(), Buffer() {}
	};
	std::vector<CodeBuffer> m_vecBuffer;

public:
	void WriteCode(UINT _BufferIdx, const std::string_view _strCode = std::string_view());
	inline void WriteCode(UINT _BufferIdx, const std::wstring_view _wstrCode);

	inline void OpenBracket(UINT _BufferIdx);
	inline void CloseBracket(UINT _BufferIdx, bool _bAddSemiColon = false);
	inline void CloseBracketAll(UINT _BufferIdx);

	inline void IncludeFile(UINT _BufferIdx, const std::string_view _strIncludeFileName);
	inline void IncludeFile(UINT _BufferIdx, const std::wstring_view _strIncludeFileName);

	inline void AddIndentation(UINT _BufferIdx);
	inline void SubIndentation(UINT _BufferIdx);
	inline int GetIndentation(UINT _BufferIdx);

public:
	template <typename T>
	HRESULT SaveAll(stdfs::path const& _path);

private:
	bool IsAvailable(UINT _BufferIdx) { return (m_vecBuffer.size() > _BufferIdx); }

	template <typename T>
	HRESULT Save(UINT _BufferIdx, std::basic_ofstream<T>& _ofstream);
};

inline void cCodeWriter::WriteCode(UINT _BufferIdx, const std::wstring_view _wstrCode)
{
	string converted = StringConv::ConvertUnicodeToUTF8(_wstrCode);
	WriteCode(_BufferIdx, converted);
}

inline void cCodeWriter::OpenBracket(UINT _BufferIdx)
{
	//먼저 중괄호를 추가한뒤
	WriteCode(_BufferIdx, "{");

	//들여쓰기 값을 늘려준다.
	++m_vecBuffer[_BufferIdx].iIndentation;
}


inline void cCodeWriter::CloseBracket(UINT _BufferIdx, bool _bAddSemiColon)
{
	//인덱스 사용가능여부 체크
	if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (UINT)1);

	//먼저 들여쓰기를 뺴준뒤
	--m_vecBuffer[_BufferIdx].iIndentation;

	if (m_vecBuffer[_BufferIdx].iIndentation < 0)
		m_vecBuffer[_BufferIdx].iIndentation = 0;

	//WriteCode 호출
	if (true == _bAddSemiColon)
		WriteCode(_BufferIdx, "};");
	else
		WriteCode(_BufferIdx, "}");
}


inline void cCodeWriter::CloseBracketAll(UINT _BufferIdx)
{
	//인덱스 사용가능여부 체크
	if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (UINT)1);

	while (0 < m_vecBuffer[_BufferIdx].iIndentation)
	{
		CloseBracket(_BufferIdx);
	}
}


inline void cCodeWriter::IncludeFile(UINT _BufferIdx, const std::string_view _strIncludeFileName)
{
	std::string strInclude = define_Preset::Keyword::IncludeBegin::A;
	strInclude += _strIncludeFileName;
	strInclude += "\"";

	WriteCode(_BufferIdx, strInclude);
}

inline void cCodeWriter::IncludeFile(UINT _BufferIdx, const std::wstring_view _wstrIncludeFileName)
{
	std::string converted = StringConv::ConvertUnicodeToUTF8(_wstrIncludeFileName);
	IncludeFile(_BufferIdx, converted);
}

inline void cCodeWriter::AddIndentation(UINT _BufferIdx)
{
	if (IsAvailable(_BufferIdx))
	{
		++m_vecBuffer[_BufferIdx].iIndentation;
	}
}

inline void cCodeWriter::SubIndentation(UINT _BufferIdx)
{
	if (IsAvailable(_BufferIdx))
	{
		--m_vecBuffer[_BufferIdx].iIndentation;
		if (0 > m_vecBuffer[_BufferIdx].iIndentation)
			m_vecBuffer[_BufferIdx].iIndentation = 0;
	}
}

inline int cCodeWriter::GetIndentation(UINT _BufferIdx)
{
	if (IsAvailable(_BufferIdx)) 
	{ 
		return m_vecBuffer[_BufferIdx].iIndentation;
	}
	else
	{
		return -1;
	}
}




template <typename T>
inline HRESULT cCodeWriter::Save(UINT _BufferIdx, std::basic_ofstream<T>& _ofstream)
{
	if (false == _ofstream.is_open())
	{
		DEBUG_BREAK;
		return ERROR_FILE_INVALID;
	}
	else if (m_vecBuffer.size() <= _BufferIdx)
	{
		DEBUG_BREAK;
		return ERROR_INDEX_ABSENT;
	}

	if constexpr (std::is_same_v<wchar_t, T>)
	{
		const string_view srcStr = m_vecBuffer[_BufferIdx].Buffer;
		wstring converted = StringConv::ConvertUTF8ToUnicode(srcStr);

		_ofstream << converted;
	}
	else if constexpr(std::is_same_v<char, T>)
	{
		_ofstream << m_vecBuffer[_BufferIdx].Buffer;
	}

	m_vecBuffer[_BufferIdx].Buffer.clear();
	m_vecBuffer[_BufferIdx].iIndentation = 0;

	return S_OK;
}

template <typename T>
inline HRESULT cCodeWriter::SaveAll(stdfs::path const& _path)
{
	std::basic_ofstream<T> ofs;
	ofs.open(_path);
	if (false == ofs.is_open())
	{
		DEBUG_BREAK;
		return E_FAIL;
	}

	for (UINT i = 0; i < m_vecBuffer.size(); ++i)
	{
		HRESULT hr = Save(i, ofs);
		if (FAILED(hr))
		{
			DEBUG_BREAK;
			return hr;
		}
	}

	ofs.close();

	m_vecBuffer.clear();
	m_vecBuffer.resize(1);

	return S_OK;
}
