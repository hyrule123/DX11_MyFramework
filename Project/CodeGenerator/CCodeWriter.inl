#include "CCodeWriter.h"

template <typename T>
inline CCodeWriter<T>::CCodeWriter()
	: m_iIndentation()
	, m_ofs()
{
}

template <typename T>
inline CCodeWriter<T>::~CCodeWriter()
{
	m_ofs.close();
}

template<typename T>
inline HRESULT CCodeWriter<T>::Open(stdfs::path const& _path)
{
	m_ofs.close();

	m_ofs.open(_path);
	
	if (false == m_ofs.is_open())
	{
		DEBUG_BREAK;
		return ERROR_FILE_HANDLE_REVOKED;
	}

	return S_OK;
}

template<typename T>
inline void CCodeWriter<T>::WriteCode(const std::basic_string_view<T> _strCode)
{
	if (false == IsOpen())
		return;

	//내려쓰기
	m_ofs << T_STRING(T, "\n");
	m_ofs.flush();
	
	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < m_iIndentation; ++i)
		m_ofs << T_STRING(T, "\t");

	//코드 작성
	m_ofs << _strCode;
}

template<typename T>
inline void CCodeWriter<T>::OpenBracket()
{
	//먼저 중괄호를 추가한뒤
	WriteCode(T_STRING(T, "{"));

	//들여쓰기 값을 늘려준다.
	++m_iIndentation;
}

template<typename T>
inline void CCodeWriter<T>::CloseBracket(bool _bAddSemiColon)
{
	//먼저 들여쓰기를 뺴준뒤
	--m_iIndentation;

	if (m_iIndentation < 0)
		m_iIndentation = 0;

	//WriteCode 호출
	if (true == _bAddSemiColon)
		WriteCode(T_STRING(T, "};"));
	else
		WriteCode(T_STRING(T, "}"));
}

template<typename T>
inline void CCodeWriter<T>::CloseBracketAll()
{
	while (0 < m_iIndentation)
	{
		CloseBracket();
	}
}

template<typename T>
inline void CCodeWriter<T>::IncludeFile(const std::basic_string<T> _strIncludeFileName)
{
	std::basic_string<T> include(T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin));
	include += _strIncludeFileName;
	include += std::char_traits<T>::to_char_type("\"");

	WriteCode(include);
}
