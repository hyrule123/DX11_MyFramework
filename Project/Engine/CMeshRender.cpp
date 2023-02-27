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

bool CMeshRender::render(eCAMERA_INDEX _eCamIdx)
{	
	//�̹��� ��µ� ī�޶� �ε����� �ڽ��� Scalar Data�� ���
	CGameObject* pOwner = GetOwner();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &(_eCamIdx));

	//�޽��� ���� �� �� �ϳ��� ���� ��� �ƿ� ���⿡ ������ �����Ƿ� ���� �˻��� �� �ʿ� ����.
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetCurMaterial();


	//������ �ڽ��� Mtrl Scalar Data�� ����� ���� ���� ���� ������ 6��(Rect Mesh)���� ���ٸ�
	//�÷��׸� ���ְ� WVP ��� ���·� �����Ѵ�.
	if (6 < pmesh->GetIdxBufferCount())
	{
		Matrix matWVP = pOwner->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD);
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, true);


		matWVP *= g_matCam[(int)_eCamIdx].matVP;
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_WVP, matWVP.m);
	}
	else
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, false);




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




		pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());

		//true�� ��ȯ�ؼ� ��ο����� ���� �ʾ����� �����Ѵ�.
		return true;
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
		
		//false�� ��ȯ�ؼ� �ν��Ͻ��� �ʿ����� �ʴٰ� ����
		return false;
	}
}