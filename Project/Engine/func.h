#pragma once


template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
			delete arr[i];
	}	
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	size_t size = _vec.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T>
void Safe_Del_List(list<T*>& _list)
{
	typename list<T*>::iterator iter = _list.begin();
	typename list<T*>::iterator iterEnd = _list.end();
	while (iter != iterEnd)
	{
		if (nullptr != (*iter))
		{
			delete (*iter);
		}
		++iter;
	}
	_list.clear();
}