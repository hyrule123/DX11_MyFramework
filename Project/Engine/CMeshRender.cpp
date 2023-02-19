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
	//메쉬와 재질 둘 중 하나라도 없을 경우 아예 여기에 들어오지 않으므로 따로 검사해 줄 필요 없음.
	Ptr<CMesh> pmesh = GetMesh();
	Ptr<CMaterial> pmtrl = GetCurMaterial();

	//Transform에 월드행렬을 반영한 WVP행렬을 업데이트해 달라고 요청
	Transform()->UpdateData();

	//인스턴싱이 설정되어있을 경우(공유 재질을 사용중일 경우)
	if (true == GetCurMaterial()->GetInstencedRender())
	{
		//재질에 자신의 Mtrl Scalar Data를 등록해 놓고
		pmtrl->AddMtrlScalarData(GetOwner()->GetMtrlScalarData());

		//카메라의 인스턴싱 대기열에 집어넣는다.
		_pCam->AddInstancingRenderQueue(pmtrl, pmesh);
	}

	else//개별 고유 재질을 사용중일 경우
	{
		CAnimator2D* pAnimator2D = Animator2D();
		if (nullptr != pAnimator2D)
			pAnimator2D->UpdateData();

		//재질에 BindData 요청 - 재질 상수버퍼가 바인딩됨.
		pmtrl->BindData();

		// 메쉬 그리기 명령
		pmesh->render(pmtrl->GetInstancingCount());
	}
}