#pragma once

#include "define_CodeGen.h"
#include "MacroFunc.h"


class CCodeWriter
{
public:
	CCodeWriter();
	~CCodeWriter();

private:
	struct CodeBuffer
	{
		int iIndentation;
		std::string Buffer;
	};
	std::vector<CodeBuffer> m_vecBuffer;

public:
	void WriteCode(const std::string_view _strCode = std::string_view(), size_t _BufferIdx = (size_t)0);
	inline void WriteCode(const std::wstring_view _wstrCode, size_t _BufferIdx = (size_t)0);

	inline void OpenBracket(size_t _BufferIdx = (size_t)0);
	inline void CloseBracket(bool _bAddSemiColon = false, size_t _BufferIdx = (size_t)0);
	inline void CloseBracketAll(size_t _BufferIdx = (size_t)0);

	inline void IncludeFile(const std::string_view _strIncludeFileName, size_t _BufferIdx = (size_t)0);
	inline void IncludeFile(const std::wstring_view _strIncludeFileName, size_t _BufferIdx = (size_t)0);

public:
	template <typename T>
	HRESULT SaveAll(stdfs::path const& _path);

private:
	bool IsAvailable(size_t _BufferIdx) { return (m_vecBuffer.size() > _BufferIdx); }

	template <typename T>
	HRESULT Save(std::basic_ofstream<T>& _ofstream, size_t _BufferIdx = (size_t)0);
};

inline void CCodeWriter::WriteCode(const std::wstring_view _wstrCode, size_t _BufferIdx)
{
	string converted = MacroFunc::ConvertUnicodeToUTF8(_wstrCode);
	WriteCode(converted, _BufferIdx);
}

inline void CCodeWriter::OpenBracket(size_t _BufferIdx)
{
	//먼저 중괄호를 추가한뒤
	WriteCode("{", _BufferIdx);

	//들여쓰기 값을 늘려준다.
	++m_vecBuffer[_BufferIdx].iIndentation;
}


inline void CCodeWriter::CloseBracket(bool _bAddSemiColon, size_t _BufferIdx)
{
	//인덱스 사용가능여부 체크
	if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (size_t)1);

	//먼저 들여쓰기를 뺴준뒤
	--m_vecBuffer[_BufferIdx].iIndentation;

	if (m_vecBuffer[_BufferIdx].iIndentation < 0)
		m_vecBuffer[_BufferIdx].iIndentation = 0;

	//WriteCode 호출
	if (true == _bAddSemiColon)
		WriteCode("};", _BufferIdx);
	else
		WriteCode("}", _BufferIdx);
}


inline void CCodeWriter::CloseBracketAll(size_t _BufferIdx)
{
	//인덱스 사용가능여부 체크
	if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (size_t)1);

	while (0 < m_vecBuffer[_BufferIdx].iIndentation)
	{
		CloseBracket(_BufferIdx);
	}
}


inline void CCodeWriter::IncludeFile(const std::string_view _strIncludeFileName, size_t _BufferIdx)
{
	std::string strInclude = define_Preset::Keyword::IncludeBegin::A;
	strInclude += _strIncludeFileName;
	strInclude += "\"";

	WriteCode(strInclude, _BufferIdx);
}

inline void CCodeWriter::IncludeFile(const std::wstring_view _wstrIncludeFileName, size_t _BufferIdx)
{
	std::string converted = MacroFunc::ConvertUnicodeToUTF8(_wstrIncludeFileName);
	IncludeFile(converted, _BufferIdx);
}




template <typename T>
inline HRESULT CCodeWriter::Save(std::basic_ofstream<T>& _ofstream, size_t _BufferIdx)
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
		wstring converted = MacroFunc::ConvertUTF8ToUnicode(srcStr);

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
inline HRESULT CCodeWriter::SaveAll(stdfs::path const& _path)
{
	std::basic_ofstream<T> ofs;
	ofs.open(_path);
	if (false == ofs.is_open())
	{
		DEBUG_BREAK;
		return E_FAIL;
	}

	for (size_t i = 0; i < m_vecBuffer.size(); ++i)
	{
		HRESULT hr = Save(ofs, i);
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
