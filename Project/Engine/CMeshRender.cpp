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

bool CMeshRender::render()
{	
	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	CGameObject* pOwner = GetOwner();

	//메쉬와 재질 둘 중 하나라도 없을 경우 아예 여기에 들어오지 않으므로 따로 검사해 줄 필요 없음.
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetCurMaterial();


	//재질에 자신의 Mtrl Scalar Data를 등록해 놓고 만약 정점 갯수가 6개(Rect )보다 많다면
	//플래그를 켜주고 WVP 행렬 형태로 전송한다.
	if (6 < pmesh->GetIdxBufferCount())
	{
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, false);

		int camIdx = pOwner->GetMtrlScalarParam_Int(MTRL_SCALAR_INT_CAMIDX);
		Matrix matWVP = pOwner->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD);
		matWVP *= g_matCam[camIdx].matVP;
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_WVP, matWVP.m);
	}
	else
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, true);




	//인스턴싱이 설정되어있을 경우(공유 재질을 사용중일 경우)
	if (true == GetCurMaterial()->GetInstencedRender())
	{

		//2D까지는 우선 W와 VP를 분리해서 전달
		//카메라 관련 행렬은 상수버퍼를 통해서 전달되었기 때문에 여기서 업데이트해줄 필요가 없어짐
		

		//3D가면 이렇게 바꿔줄것(재질에서 쉐이더에 필요한 행렬만 골라서 받을수있도록 설정해줄것.)
		//const Matrix& matWVP = pOwner->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD) * g_matCam.matWVP;
		//pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_WVP, matWVP.m);
		
		//if(...)
		//	pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_VIEW, g_matCam.matView.m);
		//if(...)
		//	pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_PROJ, g_matCam.matView.m);




		pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());

		//false를 반환해서 드로우콜을 하지 않았음을 전달한다.
		return false;
	}

	else//개별 고유 재질을 사용중일 경우
	{
		//여기서는 카메라 데이터를 따로 계산할 필요 없음 - 상수버퍼에 바인딩되어 있음.
		//여기도 나중에 3D되면 바꿔줄것


		//재질에 BindData 요청 - 재질 상수버퍼가 바인딩됨.
		pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());
		pmtrl->BindData();

		// 메쉬 그리기 명령
		UINT InstancingCount = pmtrl->GetInstancingCount();

		pmesh->renderInstanced(InstancingCount);
		
		//true를 반환해서 인스턴싱이 필요하지 않다고 전달
		return true;
	}
}
