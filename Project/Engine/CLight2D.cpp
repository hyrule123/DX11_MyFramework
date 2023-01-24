#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

//렌더링매니저에 매 프레임마다 자신의 정보를 업데이트 해야함.
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
