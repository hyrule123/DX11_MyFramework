#include "pch.h"
#include "CMeshRender.h"


#include "CGraphicsShader.h"
#include "CTransform.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(eCOMPONENT_TYPE::MESH_RENDER)		
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

	// Transform �� BindData ��û - ��������� ������� ���ε��� ����ü�� ��ϵ�.
	//ī�޶�� finaltick()���� �ڽ��� �� �� ��������� �������.
	CTransform* pTransform = Transform();
	if(nullptr != pTransform)
		pTransform->UpdateData();

	CAnimator2D* pAnimator2D = Animator2D();
	if (nullptr != pAnimator2D)
		pAnimator2D->UpdateData();

	// ������ BindData ��û - ���� ������۰� ���ε���.
	pmtrl->BindData();

	// �޽� �׸��� ���
	pmesh->render();
}