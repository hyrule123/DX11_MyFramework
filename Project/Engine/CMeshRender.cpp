#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"

CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)		
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{	
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	// Transform ¿¡ UpdateData ¿äÃ»
	Transform()->UpdateData();

	m_pMtrl->UpdateData();
	m_pMesh->render();
}