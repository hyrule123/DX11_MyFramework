#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

#include "CStructBuffer.h"

//상수버퍼 업데이트용
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CRenderMgr::CRenderMgr()
    : m_arrCam{}
    , m_bDebugRenderUpdated()
    , m_pLight2DStructBuffer()
{
}

CRenderMgr::~CRenderMgr()
{
    delete m_pLight2DStructBuffer;
}


void CRenderMgr::RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx)
{
    assert(0 <= _idx && _idx < eCAMIDX_END);
    m_arrCam[_idx] = _pCam;
}

void CRenderMgr::RemoveCamera(CCamera* _pCam)
{
    for (int i = 0; i < eCAMIDX_END; ++i)
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
    m_pLight2DStructBuffer = new CStructBuffer;
    m_pLight2DStructBuffer->Create((UINT)sizeof(tLightInfo), 10);
    m_pLight2DStructBuffer->SetPipelineTarget(eSHADER_PIPELINE_FLAG_PIXEL);
    m_pLight2DStructBuffer->SetRegisterIdx(8u);
}


void CRenderMgr::render()
{
    UpdateData();

    for (int i = 0; i < eCAMIDX_END; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //카메라에서 오브젝트를 도메인에 따라서 분류한다.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }

    //클라이언트 쪽에서 디버그렌더링 정보를 받아가지 않았을 경우 데이터 폐기
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}

void CRenderMgr::UpdateData()
{
    //글로벌 정보를 tGlobalValue 상수버퍼로 업데이트
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_GLOBAL);
    
    g_GlobalVal.Light2DCount = (UINT)m_vecLight2DStruct.size();
    g_GlobalVal.Light3DCount = 0u;

    pConstBuffer->SetData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->UpdateData();


    //자신의 구조화 버퍼 데이터를 업데이트 한후 데이터를 클리어
    m_pLight2DStructBuffer->SetData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->UpdateData();
    m_vecLight2DStruct.clear();
}
