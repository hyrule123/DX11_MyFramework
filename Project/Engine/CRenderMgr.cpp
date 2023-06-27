#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

#include "CStructBuffer.h"

//상수버퍼 업데이트용
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "CKeyMgr.h"

#include "CRenderComponent.h"

#include "CConstBuffer.h"


CRenderMgr::CRenderMgr()
    : m_arrCam{}
    , m_bDebugRenderUpdated()
    , m_pLight2DStructBuffer()
    , m_bEditorCamMode(false)
{
    //구조화버퍼 생성
    m_pSBuffer_Instancing = new CStructBuffer(tSBufferDesc{
        eSTRUCT_BUFFER_TYPE::READ_ONLY,
        define_Shader::eSHADER_PIPELINE_STAGE::__ALL,
        eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR
        , idx_t_SBUFFER_MTRL_SCALAR,
        idx_u_UAV_NONE }
    );
    m_pSBuffer_Instancing->Create((UINT)sizeof(tMtrlScalarData), 100u, nullptr, 0u);
}

CRenderMgr::~CRenderMgr()
{
    SAFE_DELETE(m_pLight2DStructBuffer);
    SAFE_DELETE(m_pSBuffer_Instancing);
}




void CRenderMgr::RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx)
{
    assert(0 <= (int)_idx && (int)_idx < (int)eCAMERA_INDEX::END);
    m_arrCam[(int)_idx] = _pCam;
}

void CRenderMgr::RemoveCamera(CCamera* _pCam)
{
    for (int i = 0; i < (int)eCAMERA_INDEX::END; ++i)
    {
        if (_pCam == m_arrCam[i])
        {
            m_arrCam[i] = nullptr;
            return;
        }
    }
}

void CRenderMgr::UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef)
{
    //렌더 매니저에 저장된 디버그 쉐이프 렌더링 목록을 복사해준뒤 자신의 목록을 비운다.
    std::copy(m_vecDebugShapeRender.begin(), m_vecDebugShapeRender.end(), std::back_inserter(_vecDebugRef));
    m_vecDebugShapeRender.clear();
    m_bDebugRenderUpdated = true;
}




void CRenderMgr::init()
{
    //광원정보는 픽셀에서만 필요, 8번 텍스처 레지스터에 바인딩 되어있음.
    m_pLight2DStructBuffer = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_ONLY, define_Shader::eSHADER_PIPELINE_STAGE::__PIXEL, eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D, idx_t_SBUFFER_LIGHT2D, idx_u_UAV_NONE
        });
    m_pLight2DStructBuffer->Create((UINT)sizeof(tLightInfo), 10, nullptr, 0u);
}

void CRenderMgr::tick()
{
    if (KEY_PRESSED(eKEY::LCTRL) && KEY_DOWN(eKEY::E))
    {
        m_bEditorCamMode = !m_bEditorCamMode;
    }
}


void CRenderMgr::render()
{
    UpdateBuffer();

    if (true == m_bEditorCamMode)
        render_editor();
    else
        render_play();


    //클라이언트 쪽에서 디버그렌더링 정보를 받아가지 않았을 경우 데이터 폐기
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}


void CRenderMgr::UpdateBuffer()
{
    //글로벌 정보를 tGlobalValue 상수버퍼로 업데이트
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_SYSTEM);

    
    pConstBuffer->UploadData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->BindBuffer();

    //자신의 구조화 버퍼 데이터를 업데이트 한후 데이터를 클리어
    m_pLight2DStructBuffer->UploadData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->BindBufferSRV();
    m_vecLight2DStruct.clear();
}

void CRenderMgr::render_editor()
{
    assert(nullptr != m_arrCam[(int)eCAMERA_INDEX::EDITOR]);

    m_arrCam[(int)eCAMERA_INDEX::EDITOR]->SortObject();

    renderAll();
}

void CRenderMgr::render_play()
{
    for (int i = 0; i < (int)eCAMERA_INDEX::EDITOR; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //카메라에서 오브젝트를 도메인에 따라서 분류한다.
        m_arrCam[i]->SortObject();
    }

    renderAll();
}

void CRenderMgr::renderAll()
{
    //카메라의 행렬을 상수버퍼에 업로드
    static CConstBuffer* const pBuffer = CDevice::GetInst()->GetConstBuffer(idx_b_CBUFFER_CAM_MATIRCES);
    pBuffer->UploadData(&g_matCam);
    pBuffer->BindBuffer();

    //2D 과정 동안에는 W와 VP를 쉐이더에서 계산하고,
    //3D 과정에 들어간 이후부터 WVP를 일괄적으로 넘겨주도록 변경할 예정,
    //그렇기 때문에 카메라를 기준으로 인스턴싱 및 렌더링을 진행함
    // 
    //2D에는 메쉬의 정점이 대부분 4개이므로 GPU와 작업을 좀 분담하려고 함
    //나중에 3D 과정 가면 코드를 변경할 것
    //나중에 혹시 별도의 렌더타겟에 렌더링하는 코드가 생길 경우
    //해당 파트도 처리해줄 방법을 가지고 있어야 함.
    for (UINT i = 0; i < (UINT)define_Shader::eSHADER_DOMAIN::_END; ++i)
    {
        size_t size = m_arrvecShaderDomain[i].size();
        for (size_t j = 0; j < size; j++)
        {
            tRenderInfo& RenderInfo = m_arrvecShaderDomain[i][j];
            //카메라 인덱스를 설정해준다.
            RenderInfo.pRenderCom->SetCamIdx(RenderInfo.pCam->GetCamIndex());

            //만약 render 메소드를 호출했는데 false가 반환되었을 경우(드로우콜 미발생 == 인스턴싱이 필요하다)
            if (false == RenderInfo.pRenderCom->render())
            {
                //인스턴싱 대기열 map에 추가
                tInstancingKey Key = {};
                Key.pMtrl = (DWORD_PTR)(RenderInfo.pRenderCom->GetCurMaterial().Get());
                Key.pMesh = (DWORD_PTR)(RenderInfo.pRenderCom->GetMesh().Get());

                const tMtrlScalarData& ScalarData = RenderInfo.pRenderCom->GetMtrlScalarData();

                AddInstancingQueue(Key, ScalarData);
            }
        }

        //모두 순회 돌았으면 인스턴싱으로 렌더링
        InstancedRender();

        //렌더링한 쉐이더 도메인은 제거
        m_arrvecShaderDomain[i].clear();
    }
}

void CRenderMgr::AddInstancingQueue(const tInstancingKey& _Key, const tMtrlScalarData& _Value)
{
    m_mapInstancing[_Key].push_back(_Value);
}

void CRenderMgr::InstancedRender()
{
    for (const auto& iter : m_mapInstancing)
    {
        UINT instances = (UINT)iter.second.size();
        m_pSBuffer_Instancing->UploadData((void*)iter.second.data(), instances);
        m_pSBuffer_Instancing->BindBufferSRV();

        CMaterial* pMtrl = (CMaterial*)(iter.first.pMtrl);
        pMtrl->BindData();

        CMesh* pMesh = (CMesh*)(iter.first.pMesh);
        pMesh->renderInstanced(instances);
    }
    //인스턴싱 대기열 클리어 
    m_mapInstancing.clear();
}
