#pragma once

struct tPtrData
{
	void*	ptr;
	size_t  size;

	tPtrData() : ptr(), size(8u) {}

	tPtrData(void* _ptr, size_t _size = 8u)
		: ptr(_ptr)
		, size(_size)
	{}
};

//콜백함수용 구조체
class CUI;
struct tUIData
{
	const CUI*	pUI;
	tPtrData	tData;

	tUIData(const CUI* _pUI, tPtrData _tData = tPtrData{})
		: pUI(_pUI)
		, tData(_tData)
	{}
};




enum class eUI_MOUSE_STATUS
{
	HOVERED,
	LBTN_CLICKED,
	LBTN_DOUBLE_CLICKED,
	END
};

//string strName, tPtrData pData
struct tComboItem
{
	string strName;
	tPtrData pData;

	tComboItem() : strName(), pData() {};

	tComboItem(const string& _strName, tPtrData _pData = { nullptr, })
		: strName(_strName)
		, pData(_pData)
	{}
};
