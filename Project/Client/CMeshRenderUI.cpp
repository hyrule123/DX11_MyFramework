#include "pch.h"
#include "CMeshRenderUI.h"

CMeshRenderUI::CMeshRenderUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CMeshRenderUI::~CMeshRenderUI()
{
}

int CMeshRenderUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
