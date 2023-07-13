#include "pch.h"
#include "Light2D.h"

#include "Transform.h"

//렌더링매니저에 매 프레임마다 자신의 정보를 업데이트 해야함.
#include "RenderMgr.h"

Light2D::Light2D()
	: Light(eCOMPONENT_TYPE::LIGHT2D)
	, m_LightInfo{}
{
	m_LightInfo.LightColor.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	m_LightInfo.LightColor.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
	//m_LightInfo.vLightDir = Vec4::Unit[eAXIS3D::X];
	m_LightInfo.fAngle = XM_PI / 2.5f;	//30 degree
	m_LightInfo.fRadius = 400.f;
}

Light2D::~Light2D()
{
}

void Light2D::finaltick()
{
	m_LightInfo.vLightWorldPos = Transform().GetWorldPos();

	//만약 스포트라이트 또는 직사광선일 경우 트랜스폼의 방향을 따라가도록 설정
	if((int)eLIGHT_TYPE::DIRECTIONAL == m_LightInfo.LightType
		||
		(int)eLIGHT_TYPE::SPOTLIGHT == m_LightInfo.LightType)
		m_LightInfo.vLightDir = Transform().GetRelativeDir(eDIR_TYPE::RIGHT);

	RenderMgr::GetInst()->AddLight2DData(m_LightInfo);
}
