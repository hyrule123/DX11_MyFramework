#pragma once

#define DECLARE_SINGLETON(_type) \
private:\
_type();\
~_type();\
static _type* m_Inst;\
public:\
static _type* GetInst() { if(nullptr == m_Inst) m_Inst = new _type; return m_Inst; };\
static void Destroy() { if(m_Inst) { delete m_Inst; m_Inst = nullptr; } }

#define DEFINITION_SINGLETON(_type) \
_type* _type::m_Inst = nullptr


//#define SINGLETON(type) \
//friend class Singleton<type>;\
//private:\
//type();\
//~type()
//
//typedef void (*EXIT)(void);
//
//template<typename T>
//class Singleton
//{
//private:
//	static T* m_Inst;
//
//public:
//	static T* GetInst();
//	static void Destroy();
//
//protected:
//	Singleton() { }
//	virtual ~Singleton() {}	
//};
//
//template<typename T>
//T* Singleton<T>::m_Inst = nullptr;
//
//template<typename T>
//inline T* Singleton<T>::GetInst()
//{
//	if (nullptr == m_Inst)
//	{
//		m_Inst = new T;
//		atexit( (EXIT) &Singleton<T>::Destroy);
//	}
//
//	return m_Inst;
//}
//
//template<typename T>
//inline void Singleton<T>::Destroy()
//{
//	if (nullptr != m_Inst)
//	{
//		delete m_Inst;
//		m_Inst = nullptr;
//	}
//}
