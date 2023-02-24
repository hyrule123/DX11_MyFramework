#include "pch.h"
#include "CMeshRender.h"


#include "CGraphicsShader.h"
#include "CTransform.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CAnimator2D.h"

#include "CCamera.h"

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

void CMeshRender::render(CCamera* _pCam)
{	
	//�޽��� ���� �� �� �ϳ��� ���� ��� �ƿ� ���⿡ ������ �����Ƿ� ���� �˻��� �� �ʿ� ����.
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetCurMaterial();

	//Transform�� ��������� �ݿ��� WVP����� ������Ʈ�� �޶�� ��û
	//Transform()->UpdateData();

	//�ν��Ͻ��� �����Ǿ����� ���(���� ������ ������� ���)
	if (true == GetCurMaterial()->GetInstencedRender())
	{
		//������ �ڽ��� Mtrl Scalar Data�� ����� ����
		pmtrl->AddMtrlScalarData(GetOwner()->GetMtrlScalarData());

		//ī�޶��� �ν��Ͻ� ��⿭�� ����w�ִ´�.
		_pCam->AddInstancingRenderQueue(pmtrl, pmesh);
	}

	else//���� ���� ������ ������� ���
	{
		//������ BindData ��û - ���� ������۰� ���ε���.
		pmtrl->AddMtrlScalarData(GetOwner()->GetMtrlScalarData());
		pmtrl->BindData();

		// �޽� �׸��� ���
		UINT InstancingCount = pmtrl->GetInstancingCount();

		pmesh->renderInstanced(InstancingCount);
	}
}