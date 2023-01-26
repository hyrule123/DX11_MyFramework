#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

//�������Ŵ����� �� �����Ӹ��� �ڽ��� ������ ������Ʈ �ؾ���.
#include "CRenderMgr.h"

CLight2D::CLight2D()
	: CLight(eCOMPONENT_LIGHT2D)
	, m_LightInfo{}
{
	m_LightInfo.LightColor.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	m_LightInfo.LightColor.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
	//m_LightInfo.vLightDir = Vec4::Unit[eAXIS3D_X];
	m_LightInfo.fAngle = XM_PI / 3.f;	//30 degree
	m_LightInfo.fRadius = 400.f;
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	m_LightInfo.vLightWorldPos = Transform()->GetWorldPos();

	//���� ����Ʈ����Ʈ �Ǵ� ���籤���� ��� Ʈ�������� ������ ���󰡵��� ����
	if((UINT)eLIGHT_TYPE::eLIGHT_DIRECTIONAL == m_LightInfo.LightType
		||
		(UINT)eLIGHT_TYPE::eLIGHT_SPOTLIGHT == m_LightInfo.LightType)
		m_LightInfo.vLightDir = Transform()->GetRelativeDir(eDIR_RIGHT);

	CRenderMgr::GetInst()->AddLight2DData(m_LightInfo);
}
