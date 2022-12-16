#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"

CMeshRender::CMeshRender()
	: CComponent(eCOMPONENT_TYPE::MESHRENDER)		
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

	// Transform �� UpdateData ��û - ��������� ������� ���ε��� ����ü�� ��ϵ�.
	//ī�޶�� finaltick()���� �ڽ��� �� �� ��������� �������.
	Transform()->UpdateData();

	// ������ UpdateData ��û - ���� ������۰� ���ε���.
	m_pMtrl->UpdateData();
	m_pMesh->render();
}