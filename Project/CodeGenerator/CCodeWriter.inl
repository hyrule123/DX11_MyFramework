#include "CCodeWriter.h"

template <typename T>
inline CCodeWriter<T>::CCodeWriter()
	: m_ofs()
	//일단 하나만 생성
	, m_vecBuffer(1)
{
}

template <typename T>
inline CCodeWriter<T>::~CCodeWriter()
{
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
inline HRESULT CCodeWriter<T>::Save(size_t _BufferIdx)
{
	if (false == IsOpen())
	{
		DEBUG_BREAK;
		return ERROR_FILE_NOT_FOUND;
	}
		
	else if (m_vecBuffer.size() <= _BufferIdx)
	{
		DEBUG_BREAK;
		return ERROR_INDEX_ABSENT;
	}

	m_ofs << m_vecBuffer[_BufferIdx].Buffer;

	m_vecBuffer[_BufferIdx].Buffer.clear();
	m_vecBuffer[_BufferIdx].iIndentation = 0;

	return S_OK;
}

template<typename T>
inline HRESULT CCodeWriter<T>::SaveAll()
{
	for (size_t i = 0; i < m_vecBuffer.size(); ++i)
	{
		HRESULT hr = Save(i);
		if (FAILED(hr))
		{
			DEBUG_BREAK;
			return hr;
		}
	}

	m_ofs.close();
	m_vecBuffer.clear();
	m_vecBuffer.resize(1);

	return S_OK;
}

template<typename T>
inline void CCodeWriter<T>::WriteCode(const std::basic_string_view<T> _strCode, size_t _BufferIdx)
{
	if (false == IsOpen())
		return;
	else if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (size_t)1);

	//내려쓰기
	m_vecBuffer[_BufferIdx].Buffer += T_STRING(T, "\n");
	
	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < m_vecBuffer[_BufferIdx].iIndentation; ++i)
		m_vecBuffer[_BufferIdx].Buffer += T_STRING(T, "\t");

	//코드 내용 추가
	m_vecBuffer[_BufferIdx].Buffer += _strCode;
}

template<typename T>
inline void CCodeWriter<T>::OpenBracket(size_t _BufferIdx)
{
	//먼저 중괄호를 추가한뒤
	WriteCode(T_STRING(T, "{"), _BufferIdx);

	//들여쓰기 값을 늘려준다.
	++m_vecBuffer[_BufferIdx].iIndentation;
}

template<typename T>
inline void CCodeWriter<T>::CloseBracket(bool _bAddSemiColon, size_t _BufferIdx)
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
		WriteCode(T_STRING(T, "};"), _BufferIdx);
	else
		WriteCode(T_STRING(T, "}"), _BufferIdx);
}

template<typename T>
inline void CCodeWriter<T>::CloseBracketAll(size_t _BufferIdx)
{
	//인덱스 사용가능여부 체크
	if (false == IsAvailable(_BufferIdx))
		m_vecBuffer.resize(_BufferIdx + (size_t)1);

	while (0 < m_vecBuffer[_BufferIdx].iIndentation)
	{
		CloseBracket(_BufferIdx);
	}
}

template<typename T>
inline void CCodeWriter<T>::IncludeFile(const std::basic_string<T> _strIncludeFileName, size_t _BufferIdx)
{
	std::basic_string<T> include(T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin));
	include += _strIncludeFileName;
	include += T_STRING(T, "\"");

	WriteCode(include);
}

