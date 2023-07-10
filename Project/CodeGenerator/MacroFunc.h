#pragma once

#include "define_CodeGen.h"

#define STATIC_ASSERT_IS_STRING static_assert(std::is_same_v<T, char> || std::is_same_v<T, wchar_t>)

class MacroFunc
{
public:
	static void ConvertUTF8ToUnicode(__in const string_view _src, __out wstring& _dest);
	static void ConvertUnicodeToUTF8(__in const wstring_view _src, __out string& _dest);

	static string ConvertUnicodeToUTF8(const wstring_view _src);
	static wstring ConvertUTF8ToUnicode(const string_view _src);
	
	inline static string UpperCase(const string_view _str);
	inline static wstring UpperCase(const wstring_view _wstr);

	inline static string& UpperCase(string& _str);
	inline static wstring& UpperCase(wstring& _wstr);

private:
	MacroFunc() = delete;
	~MacroFunc() = delete;
};

inline string MacroFunc::UpperCase(const string_view _str)
{
	string converted(_str);
	
	size_t size = converted.size();
	for (size_t i = 0; i < size; ++i)
	{
		//첫 번째 비트가 0일 경우 ASCII 코드
		if ((char)0 == (converted[i] >> 7))
		{
			converted[i] = std::toupper(converted[i]);
		}
	}
	
	return converted;
}

inline wstring MacroFunc::UpperCase(const wstring_view _wstr)
{
	wstring converted(_wstr);

	size_t size = sizeof(converted);
	for (size_t i = 0; i < size; ++i)
	{
		converted[i] = (wchar_t)std::toupper((wchar_t)converted[i]);
	}

	return converted;
}

inline string& MacroFunc::UpperCase(string& _str)
{
	size_t size = _str.size();
	for (size_t i = 0; i < size; ++i)
	{
		//첫 번째 비트가 0일 경우 ASCII 코드
		if ((char)0 == (_str[i] >> 7))
		{
			_str[i] = std::toupper(_str[i]);
		}
	}

	return _str;
}

inline wstring& MacroFunc::UpperCase(wstring& _wstr)
{
	size_t size = _wstr.size();
	for (size_t i = 0; i < size; ++i)
	{
		_wstr[i] = (wchar_t)std::toupper((int)_wstr[i]);
	}

	return _wstr;
}
