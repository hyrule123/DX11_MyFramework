#pragma once


#define SAFE_DELETE(Ptr) if(Ptr) { delete Ptr; Ptr = nullptr; }
#define SAFE_DELETE_ARRAY(Ptr) if(Ptr) { delete[] Ptr; }

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
	const typename list<T*>::iterator& iterEnd = _list.end();
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

class CGameObject;
void SpawnGameObject(CGameObject* _pNewObject, Vec3 _vWorldPos, int _LayerIdx);
void DestroyObject(CGameObject* _pObject);
void AddChild(CGameObject* _pParent, CGameObject* _pChild);

bool ConvertMultibyteToUnicode(__in const string& _src, __out wstring& _dest);
bool ConvertUnicodeToMultibyte(__in const wstring& _src, __out string& _dest);
bool ConvertUnicodeToUTF8(__in const wstring& _src, __out string& _dest);

wstring ConvertMultibyteToUnicode(const string& _src);
string ConvertUnicodeToMultibyte(const wstring& _src);
string ConvertUnicodeToUTF8(const wstring& _src);
