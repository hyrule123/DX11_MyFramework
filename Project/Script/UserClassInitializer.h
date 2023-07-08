#pragma once

#include <Engine/ptr.h>
class UserClassInitializer
{
public:
	static void InitScript();
	static void InitCS();

private:
	template <typename T>
	static Ptr<T> Constructor_Ptr_T() { return new T; }

	template <typename T>
	static T* Constructor_T() { return new T; }
};



