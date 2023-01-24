#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

//�������Ŵ����� �� �����Ӹ��� �ڽ��� ������ ������Ʈ �ؾ���.
#include "CRenderMgr.h"

CLight2D::CLight2D()
	: CLight(eCOMPONENT_LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	m_LightInfo.vLightWorldPos = Transform()->GetWorldPos();

	CRenderMgr::GetInst()->AddLight2DData(m_LightInfo);
}
