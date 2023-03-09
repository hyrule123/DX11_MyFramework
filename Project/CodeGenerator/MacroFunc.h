#pragma once


constexpr const wchar_t BracketStart = L'{';
constexpr const wchar_t BracketEnd = L'}';
constexpr const wchar_t* IncludeBegin = L"#include \"";


//�ƹ� ���� �Է����� ���� ��� �� �� ����
void WriteCode(std::wofstream& _pFile, const wstring& _strCode = L"");

inline void IncludeFile(std::wofstream& _pFile, const wstring& _strIncludeFileName)
{
	wstring include = IncludeBegin;
	include += _strIncludeFileName + L'\"';

	WriteCode(_pFile, include);
}