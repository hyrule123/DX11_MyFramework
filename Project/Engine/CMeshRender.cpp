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

	// Transform 에 BindData 요청 - 월드행렬이 상수버퍼 바인딩용 구조체에 등록됨.
	//카메라는 finaltick()에서 자신의 뷰 및 투영행렬을 등록했음.
	CTransform* pTransform = Transform();
	if(nullptr != pTransform)
		pTransform->UpdateData();

	CAnimator2D* pAnimator2D = Animator2D();
	if (nullptr != pAnimator2D)
		pAnimator2D->UpdateData();

	// 재질에 BindData 요청 - 재질 상수버퍼가 바인딩됨.
	pmtrl->BindData();

	// 메쉬 그리기 명령
	pmesh->render();
}