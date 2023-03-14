#pragma once

struct tPtrData
{
	void*	ptr;
	size_t  size;
};

//콜백함수용 구조체
class CUI;
struct tUIData
{
	const CUI*	pUI;
	tPtrData	tData;
};




enum class eUI_MOUSE_STATUS
{
	HOVERED,
	LBTN_CLICKED,
	LBTN_DOUBLE_CLICKED,
	END
};
