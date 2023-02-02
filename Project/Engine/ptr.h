#pragma once

template<typename T>
class Ptr
{
public:
	Ptr() throw()
		: m_Res(nullptr)
	{}

	Ptr(T* _Res) throw()
		: m_Res(_Res)
	{
		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	template <class U>
	Ptr(U* _Res) throw()
		: m_Res(static_cast<T*>(_Res))
	{
		InternalAddRef();
	}
	

	Ptr(const Ptr& _Res) throw()
		: m_Res(_Res.m_Res)
	{
		InternalAddRef();
	}

	template<class U>
	Ptr(const Ptr<U>& _other) throw()
		: m_Res(static_cast<T*>(_other.m_Res))
	{
		InternalAddRef();
	}


	//이동 연산자의 경우 
	//나한테 nullptr를 준 뒤 swap
	Ptr(Ptr&& _other) throw()
		: m_Res(nullptr)
	{
		if (this != reinterpret_cast<Ptr*>(&reinterpret_cast<unsigned char&>(_other)))
		{
			Swap(_other);
		}
	}


	//다른 타입의 이동 연산자도 마찬가지
	//자신의 포인터로 모든 값이 옮겨졌을 것이므로
	//반대편의 invalid한 값에 nullptr을 넣어서 에러를 방지
	template<class U>
	Ptr(Ptr<U>&& _other) throw()
		: m_Res(static_cast<T*>(_other.m_Res))
	{
		_other.m_Res = nullptr;
	}


	~Ptr()
	{
		InternalRelease();
	}


private:
	T*		m_Res;

	template<class U> 
	friend class Ptr;



public:
	T* Get() const { return m_Res; }

	T* operator -> ()
	{
		return m_Res;
	}




	//Assign Operator
	void operator = (decltype(nullptr))
	{
		InternalRelease();
	}

	template <class U>
	void operator=(U* _other)
	{
		Ptr(_other).Swap(*this);
	}

	void operator=(const Ptr& _other)
	{
		if (m_Res != _other.m_Res)
		{
			Ptr(_other).Swap(*this);
		}
	}

	template<class U>
	void operator=(const Ptr<U>& _other)
	{
		ComPtr(_other).Swap(*this);
	}

	void operator=(Ptr&& _other)
	{
		Ptr(static_cast<Ptr&&>(_other)).Swap(*this);
	}

	template<class U>
	void operator=(Ptr<U>&& _other)
	{
		Ptr(static_cast<Ptr<U>&&>(_other)).Swap(*this);
		return *this;
	}


	bool operator == (T* _other)
	{
		return m_Res == _other;			
	}

	bool operator != (T* _other)
	{
		return m_Res != _other;
	}

	bool operator == (const Ptr<T>& _other)
	{
		return m_Res == _other.m_Res;
	}

	bool operator != (const Ptr<T>& _other)
	{
		return m_Res != _other.m_Res;
	}




private:
	void InternalAddRef() const
	{
		if (m_Res != nullptr)
		{
			m_Res->AddRef();
		}
	}

	void InternalRelease()
	{
		if (nullptr != m_Res)
		{
			m_Res->Release();
			m_Res = nullptr;
		}
	}

	void Swap(Ptr& _other)
	{
		T* Temp = m_Res;
		m_Res = _other.m_Res;
		_other.m_Res = Temp;
	}

	void Swap(Ptr&& _other)
	{
		T* Temp = m_Res;
		m_Res = _other.m_Res;
		_other.m_Res = Temp;
	}
};


template<typename T>
bool operator == (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res == _Ptr.Get();
}

template<typename T>
bool operator != (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res != _Ptr.Get();
}

