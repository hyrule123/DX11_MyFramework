#pragma once

#include <Engine/ptr.h>
#include <Engine/IRes.h>
#include <Engine/IScript.h>

class UserClassInitializer
{
public:
	static void InitScript();

	//일단 미사용
	static void InitCS() {};

private:
	template <typename T>
	static Ptr<T> Constructor_Ptr_T() { static_assert(std::is_base_of_v<IRes, T>); return new T; }

	template <typename T>
	static T* Constructor_Script_T(const std::string_view _strKey) { static_assert(std::is_base_of_v<IScript, T>); return new T(_strKey); }

	template <typename T>
	static T* Constructor_T() { return new T; }
};



