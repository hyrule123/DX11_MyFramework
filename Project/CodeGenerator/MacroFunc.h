#pragma once

#include "define_CodeGen.h"


#define STATIC_ASSERT_IS_STRING static_assert(std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char8_t>)

class MacroFunc
{
public:
	template <typename T>
	inline static std::basic_string<T>& UpperCase(std::basic_string<T>& _str);

	template <typename T>
	inline static std::basic_string<T>& LowerCase(std::basic_string<T>& _str);

	template <typename T>
	inline static std::basic_string<T> UpperCase(const std::basic_string_view<T> _str);

	template <typename T>
	inline static std::basic_string<T> LowerCase(const std::basic_string_view<T> _str);

//리터럴 문자열에 사용
#define T_STRING(T, _str) MacroFunc::T_StringView<T>(_str, L##_str, u8##_str)
#define T_C_STRING(T, _Cstr) MacroFunc::T_CStringView<T>(_Cstr, L##_Cstr, u8##_Cstr)

//define_Preset에 정의된 constexpr inline const char* 타입 변수에 사용
#define T_PRESET_STR(T, _PresetStr) MacroFunc::T_StringView<T>(_PresetStr##::A, _PresetStr##::W, _PresetStr##::U8)
#define T_C_PRESET_STR(T, _PresetSTr) MacroFunc::T_CStringView<T>(_PresetStr##::A, _PresetStr##::W, _PresetStr##::U8)

//한글자
//#define T_CHAR(T, _char) std::char_traits<T>::to_char_type(_char)

//새로운 T타입 문자열 생성
#define NEW_T_STRING(T, _str) std::basic_string<T>(T_STRING(T, _str))


//SFINAE Overloaded Strings
//위 아래 함수는 enable_if와 enable_if_t 사용 예시(둘이 같은 기능임)
//현재는 SFINAE를 이용했지만, C++ 17 이후로는 if constexpr을 이용할 수도 있는 듯
	template <typename T, typename std::enable_if<std::is_same<T, char>::value, void>::type* = nullptr>
	inline static const std::basic_string_view<T> T_StringView(const std::string_view _AstrView, const std::wstring_view _WstrView, const std::u8string_view _U8strView)
	{
		STATIC_ASSERT_IS_STRING;
		return _AstrView;
	}

	template <typename T, typename std::enable_if_t<std::is_same_v<T, wchar_t>, void>* = nullptr>
	inline static const std::basic_string_view<T> T_StringView(const std::string_view _AstrView, const std::wstring_view _WstrView, const std::u8string_view _U8strView)
	{
		STATIC_ASSERT_IS_STRING;
		return _WstrView;
	}

	template <typename T, typename std::enable_if<std::is_same<T, char>::value, void>::type* = nullptr>
	inline static const T* T_CStringView(const char* _Astr, const wchar_t* _Wstr, const char8_t* _U8str)
	{
		STATIC_ASSERT_IS_STRING;
		return _Astr;
	}

	template <typename T, typename std::enable_if_t<std::is_same_v<T, wchar_t>, void>* = nullptr>
	inline static const T* T_CStringView(const char* _Astr, const wchar_t* _Wstr, const char8_t* _U8str)
	{
		STATIC_ASSERT_IS_STRING;
		return _Wstr;
	}

	
	//변수명에 사용 불가능한 특수문자들을 변환(, . ( {... 등등
	template <typename T>
	inline static std::basic_string<T> ConvertToVarName(const std::basic_string_view<T> _strView);

private:
	MacroFunc() = delete;
	~MacroFunc() = delete;
};

template<typename T>
inline std::basic_string<T>& MacroFunc::UpperCase(std::basic_string<T>& _str)
{
	STATIC_ASSERT_IS_STRING;
	if constexpr (std::is_same_v<char, T>)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
	}
	else if constexpr (std::is_same_v<wchar_t, T>)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::towupper);
	}
	else
	{
		static_assert(!std::is_same_v<char8_t, T>);
	}

	return _str;
}


template<typename T>
inline std::basic_string<T>& MacroFunc::LowerCase(std::basic_string<T>& _str)
{
	STATIC_ASSERT_IS_STRING;
	if constexpr (std::is_same_v<char, T>)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
	}
	else if constexpr (std::is_same_v<wchar_t, T>)
	{
		std::transform(_str.begin(), _str.end(), _str.begin(), ::towlower);
	}
	else
	{
		static_assert(!std::is_same_v<char8_t, T>);
	}

	return _str;
}

template<typename T>
inline std::basic_string<T> MacroFunc::UpperCase(const std::basic_string_view<T> _str)
{
	STATIC_ASSERT_IS_STRING;
	std::basic_string<T> newStr(_str);

	if constexpr (std::is_same_v<char, T>)
	{
		std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::toupper);
	}
	else if constexpr (std::is_same_v<wchar_t, T>)
	{
		std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::towupper);
	}
	else
	{
		static_assert(!std::is_same_v<char8_t, T>);
	}

	return newStr;
}

template<typename T>
inline std::basic_string<T> MacroFunc::LowerCase(const std::basic_string_view<T> _str)
{
	STATIC_ASSERT_IS_STRING;
	std::basic_string<T> newStr(_str);

	if constexpr (std::is_same_v<char, T>)
	{
		std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::tolower);
	}
	else if constexpr (std::is_same_v<wchar_t, T>)
	{
		std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::towlower);
	}
	else
	{
		static_assert(!std::is_same_v<char8_t, T>);
	}

	return newStr;
}

template<typename T>
inline std::basic_string<T> MacroFunc::ConvertToVarName(const std::basic_string_view<T> _strView)
{
	//문자열을 복사
	std::basic_string<T> Conv(_strView);

	//필터에 들어가있는 문자의 갯수를 확인
	using namespace define_Preset::VarNameFilter;
	if constexpr (std::is_same<char, T>::value)
	{
		constexpr size_t filterSize = sizeof(FiltersA) / sizeof(FiltersA[0]);

		//필터 안에 들어가 있는 문자들을 변수에서 사용해도 되는 문자인 '_'로 변환
		std::replace_if(Conv.begin(), Conv.end(),
			[](char c)->bool
			{
				for (size_t i = 0; i < filterSize; ++i)
				{
					if (FiltersA[i] == c)
						return true;
				}

				return false;

			},
			'_'
		);

	}
	else if constexpr (std::is_same<wchar_t, T>::value)
	{
		constexpr size_t filterSize = sizeof(FiltersW) / sizeof(FiltersW[0]);

		//필터 안에 들어가 있는 문자들을 변수에서 사용해도 되는 문자인 '_'로 변환
		std::replace_if(Conv.begin(), Conv.end(),
			[](wchar_t c)->bool
			{
				for (size_t i = 0; i < filterSize; ++i)
				{
					if (FiltersW[i] == c)
						return true;
				}

				return false;

			},
			'_'
		);
	}
	else if constexpr (std::is_same<wchar_t, T>::value)
	{
		constexpr size_t filterSize = sizeof(FiltersU8) / sizeof(FiltersU8[0]);

		//필터 안에 들어가 있는 문자들을 변수에서 사용해도 되는 문자인 '_'로 변환
		std::replace_if(Conv.begin(), Conv.end(),
			[](char8_t c)->bool
			{
				for (size_t i = 0; i < filterSize; ++i)
				{
					if (FiltersU8[i] == c)
						return true;
				}

				return false;

			},
			'_'
		);
	}


	return Conv;
}

