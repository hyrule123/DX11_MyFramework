#include "pch.h"
#include "CMeshRender.h"


#include "CGraphicsShader.h"
#include "CTransform.h"

#include "CMesh.h"
#include "CMaterial.h"

CMeshRender::CMeshRender()
	: CRenderComponent(eCOMPONENT_TYPE::eCOMPONENT_MESH_RENDER)		
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
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetMaterial();

	if (nullptr == pmesh || nullptr == pmtrl)
		return;

	// Transform �� UpdateData ��û - ��������� ������� ���ε��� ����ü�� ��ϵ�.
	//ī�޶�� finaltick()���� �ڽ��� �� �� ��������� �������.
	Transform()->UpdateData();

	// ������ UpdateData ��û - ���� ������۰� ���ε���.
	pmtrl->UpdateData();

	// �޽� �׸��� ���
	pmesh->render();
	
}