#include "pch.h"
#include "cRenderMgr.h"

#include "cCom_Camera.h"

#include "cStructBuffer.h"

//상수버퍼 업데이트용
#include "cTimeMgr.h"

#include "cEngine.h"
#include "cDevice.h"
#include "cConstBuffer.h"

#include "cKeyMgr.h"

#include "IRenderer.h"

#include "cConstBuffer.h"

constexpr int MAX_LIGHTS = 10;


cRenderMgr::cRenderMgr()
    : m_arrCam{}
    , m_bDebugRenderUpdated()
    , m_pcLight2DStructBuffer()
    , m_bEditorCamMode(false)
    , m_drawCallCount()
{
}

cRenderMgr::~cRenderMgr()
{
}


void cRenderMgr::RegisterCamera(cCom_Camera* _pCam, eCAMERA_INDEX _idx)
{
    assert(0 <= (int)_idx && (int)_idx < (int)eCAMERA_INDEX::END);
    m_arrCam[(int)_idx] = _pCam;
}

void cRenderMgr::RemoveCamera(cCom_Camera* _pCam)
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

void cRenderMgr::UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef)
{
    //렌더 매니저에 저장된 디버그 쉐이프 렌더링 목록을 복사해준뒤 자신의 목록을 비운다.
    std::copy(m_vecDebugShapeRender.begin(), m_vecDebugShapeRender.end(), std::back_inserter(_vecDebugRef));
    m_vecDebugShapeRender.clear();
    m_bDebugRenderUpdated = true;
}




void cRenderMgr::Init()
{
    
    {
        //인스턴싱용 구조화 버퍼 생성
        tSBufferDesc Desc = {};
        Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__ALL;
        Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_ONLY;
        Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_MTRL_SCALAR;
        Desc.REGISLOT_u_UAV = REGISLOT_u_UAV_NONE;

        m_pSBuffer_Instancing = std::make_unique<cStructBuffer>();
        m_pSBuffer_Instancing->SetDesc(Desc);
        m_pSBuffer_Instancing->Create((UINT)sizeof(tMtrlScalarData), 100u, nullptr, 0u);
    }

    {
        //인스턴싱용 구조화 버퍼 생성
        tSBufferDesc Desc = {};
        Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_ONLY;
        Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__PIXEL;
        Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_LIGHT2D;
        Desc.REGISLOT_u_UAV = REGISLOT_u_UAV_NONE;

        m_pcLight2DStructBuffer = std::make_unique<cStructBuffer>();
        m_pcLight2DStructBuffer->SetDesc(Desc);
        m_pcLight2DStructBuffer->Create((UINT)sizeof(tLightInfo), MAX_LIGHTS, nullptr, 0u);
    }
}

void cRenderMgr::Tick()
{
    if (KEY_PRESSED(eKEY::LCTRL) && KEY_DOWN(eKEY::E))
    {
        m_bEditorCamMode = !m_bEditorCamMode;
    }
}


void cRenderMgr::Render()
{
    UpdateBuffer();

    m_drawCallCount = 0;
    if (true == m_bEditorCamMode)
        render_editor();
    else
        render_play();


    //클라이언트 쪽에서 디버그렌더링 정보를 받아가지 않았을 경우 데이터 폐기
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}


void cRenderMgr::UpdateBuffer()
{
    //글로벌 정보를 tGlobalValue 상수버퍼로 업데이트
    cConstBuffer* pConstBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_GLOBAL);

    
    pConstBuffer->UploadData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->BindBuffer();

    //자신의 구조화 버퍼 데이터를 업데이트 한후 데이터를 클리어
    m_pcLight2DStructBuffer->UploadData(static_cast<void*>(m_veccLight2DStruct.data()), (UINT)m_veccLight2DStruct.size());
    m_pcLight2DStructBuffer->BindBufferSRV();
    m_veccLight2DStruct.clear();
}

void cRenderMgr::render_editor()
{
    assert(nullptr != m_arrCam[(int)eCAMERA_INDEX::EDITOR]);

    m_arrCam[(int)eCAMERA_INDEX::EDITOR]->SortObject();

    renderAll();
}

void cRenderMgr::render_play()
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

void cRenderMgr::renderAll()
{
    //카메라의 행렬을 상수버퍼에 업로드
    static cConstBuffer* const pBuffer = cDevice::GetInst()->GetConstBuffer(REGISLOT_b_CBUFFER_CAM_MATIRCES);
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

            eRENDER_RESULT result = RenderInfo.pRenderCom->Render();
            
            if (eRENDER_RESULT::FAIL == result)
            {
                continue;
            }

            //만약 render 메소드를 호출했는데 false가 반환되었을 경우(드로우콜 미발생 == 인스턴싱이 필요하다)
            if (eRENDER_RESULT::NEED_INSTANCING == result)
            {
                //인스턴싱 대기열 map에 추가
                tInstancingKey Key = {};
                Key.pMtrl = (DWORD_PTR)(RenderInfo.pRenderCom->GetCurMaterial().Get());
                Key.pMesh = (DWORD_PTR)(RenderInfo.pRenderCom->GetMesh().Get());

                const tMtrlScalarData& ScalarData = RenderInfo.pRenderCom->GetMtrlScalarData();

                AddInstancingQueue(Key, ScalarData);
            }
            else {
                ++m_drawCallCount;
            }
        }

        //모두 순회 돌았으면 인스턴싱으로 렌더링
        InstancedRender();

        //렌더링한 쉐이더 도메인은 제거
        m_arrvecShaderDomain[i].clear();
    }
}

void cRenderMgr::AddInstancingQueue(const tInstancingKey& _Key, const tMtrlScalarData& _Value)
{
    m_mapInstancing[_Key].push_back(_Value);
}

void cRenderMgr::InstancedRender()
{
    for (const auto& iter : m_mapInstancing)
    {
        UINT instances = (UINT)iter.second.size();
        m_pSBuffer_Instancing->UploadData((void*)iter.second.data(), instances);
        m_pSBuffer_Instancing->BindBufferSRV();

        cMaterial* pMtrl = (cMaterial*)(iter.first.pMtrl);
        pMtrl->BindData();

        cMesh* pMesh = (cMesh*)(iter.first.pMesh);
        pMesh->renderInstanced(instances);
        ++m_drawCallCount;
    }
    //인스턴싱 대기열 클리어 
    m_mapInstancing.clear();
}
