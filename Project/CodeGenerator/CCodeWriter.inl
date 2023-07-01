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

	if (false == stdfs::exists(_path))
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

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

	m_ofs << CONV_CHAR(T, '\t');

	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < m_iIndentation; ++i)
		m_ofs << CONV_CHAR(T, '\t');

	//코드 작성
	m_ofs << _strCode;
}

template<typename T>
inline void CCodeWriter<T>::OpenBracket()
{
	//먼저 중괄호를 추가한뒤
	WriteCode(CONV_CHAR(T, '{'));

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
		WriteCode(CONV_STRING(T, "};"));
	else
		WriteCode(CONV_CHAR(T, '}'));
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
	std::basic_string<T> include(CONV_PRESET_STR(T, define_Preset::Keyword::IncludeBegin));
	include += _strIncludeFileName;
	include += std::char_traits<T>::to_char_type('\"');

	WriteCode(include);
}
