#pragma once

#include "define_PresetString.h"
#include "MacroFunc.h"

template <typename T>
class CCodeWriter
{
	STATIC_ASSERT_IS_STRING;
public:
	CCodeWriter();
	~CCodeWriter();

	//SFINAE Overloaded Strings
	//위 아래 함수는 enable_if와 enable_if_t 사용 예시(둘이 같은 기능임)
	//현재는 SFINAE를 이용했지만, C++ 17 이후로는 if constexpr을 이용할 수도 있는 듯
//private:
//	template <typename U = T, typename std::enable_if<std::is_same<U, char>::value, void>::type* = nullptr>
//	const std::basic_string_view<U> ConvString(const std::string_view _AstrView, const std::wstring_view _WstrView)
//	{
//		return _AstrView;
//	}
//
//	template <typename U = T, typename std::enable_if_t<std::is_same_v<U, wchar_t>, void>* = nullptr>
//	const std::basic_string_view<U> ConvString(const std::string_view _AstrView, const std::wstring_view _WstrView)
//	{
//		return _WstrView;
//	}
//
//	template <typename U = T, typename std::enable_if<std::is_same<U, char>::value, void>::type* = nullptr>
//	const U* ConvCString(const char* _Astr, const wchar_t* _Wstr)
//	{
//		return _Astr;
//	}
//
//	template <typename U = T, typename std::enable_if_t<std::is_same_v<U, wchar_t>, void>* = nullptr>
//	const U* ConvCString(const char* _Astr, const wchar_t* _Wstr)
//	{
//		return _Wstr;
//	}
//
////리터럴 문자열에 사용
//#define INTERNAL_CONV_STRING(_str) ConvString(_str, L##_str)
//#define INTERNAL_CONV_C_STRING(_Cstr) ConvCString(_str, L##_str)
//
//
////define_Preset에 정의된 constexpr inline const char* 타입 변수에 사용
//#define INTERNAL_CONV_PRESET_STR(_PresetStr) ConvString(_PresetStr##_A, _PresetStr##_W)
//#define INTERNAL_CONV_C_PRESET_STR(_PresetSTr) ConvCString(_PresetStr##_A, _PresetStr##_W)


private:
	int m_iIndentation;
	std::basic_ofstream<T> m_ofs;

public:
	HRESULT Open(stdfs::path const& _path);
	bool IsOpen() const { return m_ofs.is_open(); }


	void WriteCode(const std::basic_string_view<T> _strCode = std::basic_string_view<T>());
	//void WriteCode(const T _char) { WriteCode(std::basic_string_view<T>(&_char, sizeof(T))); }

	void OpenBracket();
	void CloseBracket(bool _bAddSemiColon = false);
	void CloseBracketAll();
	void IncludeFile(const std::basic_string<T> _strIncludeFileName);
};


#include "CCodeWriter.inl"
