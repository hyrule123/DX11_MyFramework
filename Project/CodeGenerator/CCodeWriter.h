#pragma once

#include "define_PresetString.h"
#include "MacroFunc.h"



template <typename T>
class CCodeWriter
{
	STATIC_ASSERT_IS_STRING;
private:
	struct CodeBuffer
	{
		int iIndentation;
		std::basic_string<T> Buffer;
	};

public:
	CCodeWriter();
	~CCodeWriter();

private:
	std::basic_ofstream<T> m_ofs;
	std::vector<CodeBuffer> m_vecBuffer;

public:
	HRESULT Open(stdfs::path const& _path);
	bool IsOpen() const { return m_ofs.is_open(); }

	HRESULT Save(size_t _BufferIdx = (size_t)0);
	HRESULT SaveAll();

	void WriteCode(const std::basic_string_view<T> _strCode = std::basic_string_view<T>(), size_t _BufferIdx = (size_t)0);
	void OpenBracket(size_t _BufferIdx = (size_t)0);
	void CloseBracket(bool _bAddSemiColon = false, size_t _BufferIdx = (size_t)0);
	void CloseBracketAll(size_t _BufferIdx = (size_t)0);
	void IncludeFile(const std::basic_string<T> _strIncludeFileName, size_t _BufferIdx = (size_t)0);

private:
	bool IsAvailable(size_t _BufferIdx) { return (m_vecBuffer.size() > _BufferIdx); }
};


#include "CCodeWriter.inl"
