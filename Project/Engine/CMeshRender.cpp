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

	// Transform 에 UpdateData 요청 - 월드행렬이 상수버퍼 바인딩용 구조체에 등록됨.
	//카메라는 finaltick()에서 자신의 뷰 및 투영행렬을 등록했음.
	Transform()->UpdateData();

	// 재질에 UpdateData 요청 - 재질 상수버퍼가 바인딩됨.
	m_pMtrl->UpdateData();
	m_pMesh->render();
}