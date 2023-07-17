#include "pch.h"
#include "cCom_Camera.h"

//카메라의 위치를 얻어오기 위함
#include "cCom_Transform.h"


//화면 해상도 얻어오기 위함
#include "cDevice.h"

//레이어를 받아오기 위함
#include "cLevelMgr.h"
#include "cLevel.h"
#include "cLayer.h"


//레이어 정보 받아와서 분류용
#include "IRenderer.h"
#include "cMaterial.h"

//카메라 등록
#include "cRenderMgr.h"

//tCamMatrix 버퍼 업로드 용
#include "CConstbuffer.h"

#include "cMaterial.h"
#include "cMesh.h"

cCom_Camera::cCom_Camera():
	IComponent(eCOMPONENT_TYPE::CAMERA)
	, m_AspectRatio()
	, m_eProjectionType(ePROJ_TYPE::ORTHOGRAPHY)
	, m_iCamIdx(-1)
	, m_LayerFlag(UINT32_MAX)
	, m_fZoomScale(1.f)
{
	SetProjType(m_eProjectionType);
}

cCom_Camera::cCom_Camera(const cCom_Camera& _other)
	: IComponent(_other)
	, m_AspectRatio(_other.m_AspectRatio)
	, m_eProjectionType(_other.m_eProjectionType)
	, m_matView()	// finaltick()에서 매 tick마다 계산 됨
	, m_matProj(_other.m_matProj)
	, m_iCamIdx(-1)
	, m_LayerFlag(_other.m_LayerFlag)
	, m_fZoomScale(_other.m_fZoomScale)
{
}

cCom_Camera::~cCom_Camera()
{
}


void cCom_Camera::SetProjType(ePROJ_TYPE _Type)
{
	m_eProjectionType = _Type;

	m_AspectRatio = g_GlobalVal.v2Res.x / g_GlobalVal.v2Res.y;

	switch (m_eProjectionType)
	{
	case ePROJ_TYPE::ORTHOGRAPHY:
		//===========
		//투영 -> 사실 이건 한번만 구해줘도 됨
		//===========

		//1. 투영 행렬 생성
		m_matProj = XMMatrixOrthographicLH(g_GlobalVal.v2Res.x, g_GlobalVal.v2Res.y, 1.f, 5000.f);
		break;
	case ePROJ_TYPE::PERSPECTIVE:
		//1-1. 원근 투영행렬
		m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 5000.f);
		break;
	}

	//2. 업데이트 - 카메라 별로 렌더링이 진행되므로 굳이 업데이트 할 필요가 없음.
	//g_matCam.MatProj = m_matProj;
}


void cCom_Camera::SetCamIndex(eCAMERA_INDEX _Idx)
{
	assert(_Idx <= eCAMERA_INDEX::END);

	m_iCamIdx = (int)_Idx;
	cRenderMgr::GetInst()->RegisterCamera(this, _Idx);
}



void cCom_Camera::Zoom2D(float _fScale)
{
	if (ePROJ_TYPE::ORTHOGRAPHY != m_eProjectionType)
		return;

	m_fZoomScale *= _fScale;
	//비율로 확장/축소하므로 AspecRatio는 변하지 않음.
	m_matProj = XMMatrixOrthographicLH(g_GlobalVal.v2Res.x * m_fZoomScale, g_GlobalVal.v2Res.y * m_fZoomScale, 1.f, 10000.f);
}


void cCom_Camera::init()
{
	//기본 설정으로 투영행렬을 생성
	SetProjType(m_eProjectionType);
}

void cCom_Camera::finaltick()
{
	//트랜스폼이 업데이트 되지 않았을 경우 자신도 업데이트 할 필요 없음
	if (false == Transform()->IsUpdated())
		return;

	Vec3 vCamPos = Transform()->GetWorldPos();

	//뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
	//도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
	//이동과 회전을 원래대로 되돌리는 행렬은 특정 행렬을 다시 원래 상태로 돌리는 행렬이라고 볼 수 있다.
	//(로컬 위치) * (이동행렬) * (회전행렬) = (월드 위치)
	//(월드 위치) * (이동과 회전을 되돌리는 행렬) = (로컬 위치)
	//이건 역행렬의 정의와 일치한다.
	// I * X = X
	// X * X^(-1) = I
	//그러므로 카메라의 위치와 회전만큼 ?주는 행렬은 카메라의 위치행렬과 회전행렬의 역행렬을 곱해주면 된다는 뜻이다.
	//(R * T)^(-1) = T^(-1) * R^(-1)
	//역행렬을 곱하면 순서가 반대로 됨. 주의할 것
	//또한 역행렬은 행렬마다 만들어지는 방식이 다르다. 어떤 행렬은 역행렬이 존재하지 않을 수도 있다.
	//역행렬의 존재 여부는 판별식(행렬곱 Det)으로 가능하다.
	//월드변환행렬의 경우는 역행렬이 다 존재하므로 사용할 필요는 없음.


	//1. 위치의 역행렬 -> 그냥 음수만큼 이동해주면됨
	//x축으로 a, y축으로 b, z축으로 c만큼 이동했다고 하면
	//x축으로 -a, y축으로 -b, z축으로 -c만큼 이동해주면 됨.
	/*
	1 0 0 0 
	0 1 0 0 
	0 0 1 0
	-a -b -c 1
	*/

	m_matView = Matrix::CreateTranslation(-vCamPos);


	//2. 회전
	//회전은 이동과는 역행렬의 모습은 다르지만 쉽게 구할수 있다.
	//이는 직교행렬의 성질 덕분이다.
	//직교행렬은 각 행(또는 열)끼리 수직인 행렬을 말한다.
	//이런 직교행렬은 전치행렬이 자신의 역행렬이 되는 특징을 가지고 있다.
	//회전행렬은 직교행렬이고, 역행렬은 돌렸던 걸 다시 원상복귀시키는 행렬이므로 
	//회전행렬을 직교하면 현재 회전각의 반대 방향으로 돌릴 수 있게 된다.
	//const Vec3& vecRot = Transform()->GetRelativeRot();
	//const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
	//Matrix tempmat = Matrix::CreateFromQuaternion(vecQut);
	//m_matView *= tempmat.Transpose();
	const Matrix& matRot = Transform()->GetWorldRotMat();
	m_matView *= matRot.Transpose();

	//3. transform 상수버퍼 구조체에 업데이트 -> 안함. 나중에 render때 일괄적으로 view 행렬과 proj 행렬을 곱할 예정.
	//g_matCam.matViewProj = m_matView;


	////===========
	////투영 -> 사실 이건 한번만 구해줘도 됨 -> init()으로 이동함
	////===========
	//
	////1. 투영 행렬 생성
	//const Vec2& Resolution = cDevice::GetInst()->GetRenderResolution();
	////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	////1-1. 원근 투영행렬(체험용)
	//m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
	//
	////2. 업데이트

	//이제 카메라별로 렌더링이 진행되므로, 카메라가 가지고 있는 View 행렬과 Proj 행렬을 미리 곱해 놓는다.
	//자신의 인덱스에 해당하는 카메라로 업데이트
	g_matCam[m_iCamIdx].matView = m_matView;
	g_matCam[m_iCamIdx].matProj = m_matProj;
	g_matCam[m_iCamIdx].matVP = m_matView * m_matProj;
}

void cCom_Camera::cleanup()
{
	cRenderMgr::GetInst()->RemoveCamera(this);
}


void cCom_Camera::SortObject()
{
	cRenderMgr* pRenderMgr = cRenderMgr::GetInst();

	for (UINT32 i = 0; i < MAX_LAYER; ++i)
	{
		UINT32 mask = (UINT32)1 << i;

		if (false == (mask & m_LayerFlag))
			continue;

		cLayer& cLayer = cLevelMgr::GetInst()->GetCurLevel()->GetLayer(i);

		//카메라가 출력하고자 하는 레이어의 오브젝트 리스트를 받아와서
		const vector<cGameObject*>& vecObj = cLayer.GetvecObj();

		float fDepthZ = cLayer.GetPresetZDepth();
		bool bYSorting = cLayer.GetYSortType();
		
		size_t size = vecObj.size();
		for (size_t i = 0; i < size; ++i)
		{
			//출력 담당 컴포넌트를 받아온다.
			IRenderer* pRenderCom = vecObj[i]->Renderer();

			//컴포넌트가 없거나, 컴포넌트 내부의 출력용 클래스가 등록되어있지 않을 경우 continue
			if (
				nullptr == pRenderCom
				||
				false == pRenderCom->IsRenderReady()
				||
				pRenderCom->isDisabled()
				)
				continue;
			
			
			//2D 직교 행렬을 사용중일 경우 2D 컬링 진행.
			if (ePROJ_TYPE::ORTHOGRAPHY == m_eProjectionType)
			{
				const Vec3& WorldPos = vecObj[i]->Transform()->GetWorldPos();
				float SideLen = vecObj[i]->Transform()->GetAABBSideLen();

				//스크린상에서의  위치 구하기
				const Vec3& ScreenPos = WorldPos - Transform()->GetWorldPos();

				
				const Vec2& ResHalf = g_GlobalVal.v2Res * 0.5f;
				
				if (
					//자신의 Left가 카메라의 Right보다 클 경우
					(ScreenPos.x - SideLen) > ResHalf.x
					||

					//자신의 Right가 카메라의 Left보다 클 경우
					(ScreenPos.x + SideLen) < -ResHalf.x
					||

					//자신의 Bottom이 카메라의 Top보다 클 경우
					(ScreenPos.y - SideLen) > ResHalf.y
					||

					//자신의 Top이 카메라의 Bottom보다 작을 경우
					(ScreenPos.y + SideLen) < -ResHalf.y
					)
					continue;

				//카메라가 Y소팅을 하는 지금 시점은 cCom_Transform이 이미 위치값을 cGameObject에 등록해놓은 상태임
				//게임오브젝트 내부에서 Z값을 덮어씌워줘야 함
				if (bYSorting)
				{
					vecObj[i]->YSort(fDepthZ);
				}
			}


			//쉐이더 도메인을 받아와서
			define_Shader::eSHADER_DOMAIN dom = pRenderCom->GetCurMaterial()->GetShader()->GetShaderDomain();

			//만약 쉐이더 도메인이 등록되어있지 않을 경우 assert 처리
			assert((define_Shader::eSHADER_DOMAIN)0 <= dom && dom < define_Shader::eSHADER_DOMAIN::_END);

			//도메인이 등록되어 있을 경우 cRenderMgr의 출력 대기열에 등록한다.
			pRenderMgr->AddRenderQueue(tRenderInfo{ pRenderCom, this }, dom);
		}
	}
}

