#pragma once

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
friend class CSingleton<type>;\
private:\
type();\
~type()

typedef void (*EXIT)(void);

template<typename T>
class CSingleton
{
protected:
	CSingleton() {}
	virtual ~CSingleton() {}

private:
	static T* m_Inst;

public:
	static T* GetInst();
	static void Destroy();
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = new T;
		atexit( (EXIT) &CSingleton<T>::Destroy);
	}

	return m_Inst;
}

template<typename T>
inline void CSingleton<T>::Destroy()
{
	if (nullptr != m_Inst)
	{
		delete m_Inst;
		m_Inst = nullptr;
	}
}
