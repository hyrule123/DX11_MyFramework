#pragma once


constexpr const wchar_t BracketStart = L'{';
constexpr const wchar_t BracketEnd = L'}';
constexpr const wchar_t* IncludeBegin = L"#include \"";


//아무 값도 입력하지 않을 경우 한 줄 내림
void WriteCode(std::wofstream& _pFile, const wstring& _strCode = L"");

inline void IncludeFile(std::wofstream& _pFile, const wstring& _strIncludeFileName)
{
	wstring include = IncludeBegin;
	include += _strIncludeFileName + L'\"';

	WriteCode(_pFile, include);
}