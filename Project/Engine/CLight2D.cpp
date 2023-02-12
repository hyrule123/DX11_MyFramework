#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

//�������Ŵ����� �� �����Ӹ��� �ڽ��� ������ ������Ʈ �ؾ���.
#include "CRenderMgr.h"

CLight2D::CLight2D()
	: CLight(eCOMPONENT_TYPE::LIGHT2D)
	, m_LightInfo{}
{
	m_LightInfo.LightColor.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	m_LightInfo.LightColor.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
	//m_LightInfo.vLightDir = Vec4::Unit[eAXIS3D::X];
	m_LightInfo.fAngle = XM_PI / 2.5f;	//30 degree
	m_LightInfo.fRadius = 400.f;
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	m_LightInfo.vLightWorldPos = Transform()->GetWorldPos();

	//���� ����Ʈ����Ʈ �Ǵ� ���籤���� ��� Ʈ�������� ������ ���󰡵��� ����
	if((int)eLIGHT_TYPE::DIRECTIONAL == m_LightInfo.LightType
		||
		(int)eLIGHT_TYPE::SPOTLIGHT == m_LightInfo.LightType)
		m_LightInfo.vLightDir = Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);

	CRenderMgr::GetInst()->AddLight2DData(m_LightInfo);
}
