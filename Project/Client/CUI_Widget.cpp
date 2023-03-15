#include "pch.h"
#include "CUI_Widget.h"

CUI_Widget::CUI_Widget(const string& _strName, eWIDGET_TYPE _Type)
	: CUI(_strName)
	, m_Type(_Type)

{
}

CUI_Widget::~CUI_Widget()
{
}


