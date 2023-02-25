#include "pch.h"
#include "CMeshRender.h"


#include "CGraphicsShader.h"
#include "CTransform.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CAnimator2D.h"

#include "CCamera.h"

#include "CGameObject.h"

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

bool CMeshRender::render(int _iCamIdx)
{	
	//�̹��� ��µ� ī�޶� �ε����� �ڽ��� Scalar Data�� ���
	CGameObject* pOwner = GetOwner();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &_iCamIdx);

	//�޽��� ���� �� �� �ϳ��� ���� ��� �ƿ� ���⿡ ������ �����Ƿ� ���� �˻��� �� �ʿ� ����.
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetCurMaterial();

	//Transform�� ��������� �ݿ��� WVP����� ������Ʈ�� �޶�� ��û
	//Transform()->UpdateData();

	//�ν��Ͻ��� �����Ǿ����� ���(���� ������ ������� ���)
	if (true == GetCurMaterial()->GetInstencedRender())
	{

		//2D������ �켱 W�� VP�� �и��ؼ� ����
		//ī�޶� ���� ����� ������۸� ���ؼ� ���޵Ǿ��� ������ ���⼭ ������Ʈ���� �ʿ䰡 ������
		

		//3D���� �̷��� �ٲ��ٰ�(�������� ���̴��� �ʿ��� ��ĸ� ��� �������ֵ��� �������ٰ�.)
		//const Matrix& matWVP = pOwner->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD) * g_matCam.matWVP;
		//pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_WVP, matWVP.m);
		
		//if(...)
		//	pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_VIEW, g_matCam.matView.m);
		//if(...)
		//	pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_PROJ, g_matCam.matView.m);


		//������ �ڽ��� Mtrl Scalar Data�� ����� ����
		pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());

		//false�� ��ȯ�ؼ� ��ο����� ���� �ʾ����� �����Ѵ�.
		return false;
	}

	else//���� ���� ������ ������� ���
	{
		//���⼭�� ī�޶� �����͸� ���� ����� �ʿ� ���� - ������ۿ� ���ε��Ǿ� ����.
		//���⵵ ���߿� 3D�Ǹ� �ٲ��ٰ�


		//������ BindData ��û - ���� ������۰� ���ε���.
		pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());
		pmtrl->BindData();

		// �޽� �׸��� ���
		UINT InstancingCount = pmtrl->GetInstancingCount();

		pmesh->renderInstanced(InstancingCount);
		
		//true�� ��ȯ�ؼ� ��ο����� ȣ���ߴٰ� ����
		return true;
	}
}