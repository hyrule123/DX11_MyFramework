#include "pch.h"
#include "cCom_Renderer_Basic.h"


#include "CGraphicsShader.h"
#include "cCom_Transform.h"

#include "cMesh.h"
#include "cMaterial.h"

#include "cCom_Animator2D.h"

#include "cCom_Camera.h"

#include "cGameObject.h"

cCom_Renderer_Basic::cCom_Renderer_Basic()
	: IRenderer(eCOMPONENT_TYPE::MESH_RENDER)
{
}

cCom_Renderer_Basic::~cCom_Renderer_Basic()
{
}



void cCom_Renderer_Basic::finaltick()
{
}

bool cCom_Renderer_Basic::render()
{	
	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	cGameObject* pOwner = GetOwner();

	//메쉬와 재질 둘 중 하나라도 없을 경우 아예 여기에 들어오지 않으므로 따로 검사해 줄 필요 없음.
	Ptr<cMesh> pmesh = GetMesh();
	Ptr<cMaterial> pmtrl = GetCurMaterial();


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


	//인스턴싱을 사용하는 쉐이더가 아닐경우 바로 데이터를 전송
	if(false == GetCurMaterial()->IsUseInstancing())
	{
		//여기서는 카메라 데이터를 따로 계산할 필요 없음 - 상수버퍼에 바인딩되어 있음.
		//여기도 나중에 3D되면 바꿔줄것

		//인스턴싱을 사용하지 않으므로 Scalar 데이터를 상수버퍼에 바인딩
		BindMtrlScalarDataToCBuffer();

		//재질에 BindData 요청 - 재질 상수버퍼가 바인딩됨.
		pmtrl->BindData();

		pmesh->render();
		
		//true를 반환해서 인스턴싱이 필요하지 않다고 전달
		return true;
	}

	//인스턴싱을 사용할 경우 return false를 해줌으로써 인스턴싱 처리를 해줘야 함을 전달
	return false;
}
