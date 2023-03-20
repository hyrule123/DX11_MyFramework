//#include "pch.h"
//#include "StringConvert.h"
//
//bool ConvertMultibyteToUnicode(__in const string& _src, __out wstring& _dest)
//{
//	_dest.clear();
//
//	if (_src.length() < 0)
//		return false;
//
//	int srcsize = (int)_src.size();
//	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);
//
//	if (len == 0)
//		return false;
//
//	_dest.resize(len);
//
//	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size());
//
//	return true;
//}
//
//bool ConvertUnicodeToMultibyte(__in const wstring& _src, __out string& _dest)
//{
//	_dest.clear();
//
//	if (_src.length() < 0)
//		return false;
//
//	int srcsize = (int)_src.size();
//	int len = ::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);
//
//	if (len == 0)
//		return false;
//
//	_dest.resize(len);
//
//	::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size(), nullptr, nullptr);
//
//	return true;
//}
//
//bool ConvertUnicodeToUTF8(__in const wstring& _src, __out string& _dest)
//{
//	_dest.clear();
//
//	if (_src.length() < 0)
//		return false;
//
//	int srcsize = (int)_src.size();
//	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);
//
//	if (len == 0)
//		return false;
//
//	_dest.resize(len);
//
//	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size(), nullptr, nullptr);
//
//	return true;
//}
//
//wstring ConvertMultibyteToUnicode(const string& _src)
//{
//	wstring result;
//
//	if (_src.length() < 0)
//		return result;
//
//	int srcsize = (int)_src.size();
//	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);
//
//	if (len == 0)
//		return result;
//
//	result.resize(len);
//
//	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());
//
//	return result;
//}
//
//string ConvertUnicodeToMultibyte(const wstring& _src)
//{
//	string result;
//
//	if (_src.length() < 0)
//		return result;
//
//	int srcsize = (int)_src.size();
//	int len = ::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);
//
//	if (len == 0)
//		return result;
//
//	result.resize(len);
//
//	::WideCharToMultiByte(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);
//
//	return result;
//}
//
//string ConvertUnicodeToUTF8(const wstring& _src)
//{
//	string result;
//
//	if (_src.length() < 0)
//		return result;
//
//	int srcsize = (int)_src.size();
//	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);
//
//	if (len == 0)
//		return result;
//
//	result.resize(len);
//
//	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);
//
//	return result;
//}
