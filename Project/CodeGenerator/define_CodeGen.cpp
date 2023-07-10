#include "pch.h"
#include "define_CodeGen.h"


std::regex define_Preset::Regex::g_VarForbiddenChars::A;
std::wregex define_Preset::Regex::g_VarForbiddenChars::W;

void define_Preset::Regex::g_VarForbiddenChars::CreateVarForbiddendRegex()
{
	string strBase = "[";
	wstring wstrBase = L"[";

	using namespace define_Preset::Regex;
	constexpr size_t size = sizeof(arrCharsVarForbidden::A) / sizeof(char*);
	for (size_t i = 0; i < size; ++i)
	{
		strBase += arrCharsVarForbidden::A[i];
		wstrBase += arrCharsVarForbidden::W[i];
	}

	strBase += ']';
	wstrBase += L']';

	g_VarForbiddenChars::A = std::regex(strBase);
	g_VarForbiddenChars::W = std::wregex(wstrBase);
}
