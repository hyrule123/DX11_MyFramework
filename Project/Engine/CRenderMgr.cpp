#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

#include "CStructBuffer.h"

//상수버퍼 업데이트용
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "CKeyMgr.h"

CRenderMgr::CRenderMgr()
    : m_arrCam{}
    , m_bDebugRenderUpdated()
    , m_pLight2DStructBuffer()
    , m_pEditorCam()
    , m_bEditorCamMode()
{
}

CRenderMgr::~CRenderMgr()
{
    delete m_pLight2DStructBuffer;
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
    m_pLight2DStructBuffer = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__PIXEL, eSBUFFER_SHARED_CBUFFER_IDX::LIGHT2D, eSRV_REGISTER_IDX::LIGHT2D, eUAV_REGISTER_IDX::NONE);
    m_pLight2DStructBuffer->Create((UINT)sizeof(tLightInfo), 10, nullptr, 0u);
}

void CRenderMgr::tick()
{
    if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::E))
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
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::GLOBAL);
    pConstBuffer->UploadData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->BindBuffer();

    //자신의 구조화 버퍼 데이터를 업데이트 한후 데이터를 클리어
    m_pLight2DStructBuffer->UploadData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->BindBufferSRV();
    m_vecLight2DStruct.clear();
}

void CRenderMgr::render_editor()
{
    assert(nullptr != m_pEditorCam);

    m_pEditorCam->SortObject();
    m_pEditorCam->render();
}

void CRenderMgr::render_play()
{
    for (int i = 0; i < (int)eCAMERA_INDEX::END; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //카메라에서 오브젝트를 도메인에 따라서 분류한다.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }
}
