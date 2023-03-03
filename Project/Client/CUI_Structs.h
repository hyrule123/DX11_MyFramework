#pragma once

struct tDataPtr
{
	void* pData;
	size_t  uDataSize;
};


enum class eUI_MOUSE_STATUS
{
	HOVERED,
	LBTN_CLICKED,
	LBTN_DOUBLE_CLICKED,
	END
};