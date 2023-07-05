#pragma once

#include <Engine/ptr.h>
class UserClassInitializer
{
public:
	static void InitScript();
	static void InitCS();

private:
	template <typename T>
	static Ptr<T> Constructor_T() { return new T; }
};



