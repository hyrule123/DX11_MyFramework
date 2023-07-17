#pragma once

#include <memory>

//#define DECLARE_SINGLETON(_type) \
//private:\
//_type();\
//~_type();\
//static _type* m_Inst;\
//public:\
//static _type* GetInst() { if(nullptr == m_Inst) m_Inst = new _type; return m_Inst; };\
//static void Destroy() { if(m_Inst) { delete m_Inst; m_Inst = nullptr; } }
//
//#define DEFINITION_SINGLETON(_type) \
//_type* _type::m_Inst = nullptr




#define SINGLETON(type) \
friend class Singleton<type>;\
friend std::unique_ptr<type> std::make_unique<type>();\
friend std::unique_ptr<type>::deleter_type;\
private:\
type();\
~type()

//typedef void (*EXIT)(void);

template<typename T>
class Singleton
{
	friend class std::unique_ptr<T>;
protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static std::unique_ptr<T> m_Inst;

public:
	static T* GetInst();
	//static void Destroy();
};

template<typename T>
std::unique_ptr<T> Singleton<T>::m_Inst = nullptr;

template<typename T>
inline T* Singleton<T>::GetInst()
{
	if (nullptr == m_Inst.get())
	{
		m_Inst = std::make_unique<T>();
		//atexit( (EXIT) &Singleton<T>::Destroy);
	}

	return m_Inst.get();
}

//template<typename T>
//inline void Singleton<T>::Destroy()
//{
//	if (nullptr != m_Inst)
//	{
//		delete m_Inst;
//		m_Inst = nullptr;
//	}
//}
