#include "pch.h"
#include "CRenderComponent.h"

//출력을 위한 클래스
#include "CMesh.h"
#include "CMaterial.h"


CRenderComponent::CRenderComponent(eCOMPONENT_TYPE _type):
	CComponent(_type)
{
}

CRenderComponent::~CRenderComponent()
{
}
