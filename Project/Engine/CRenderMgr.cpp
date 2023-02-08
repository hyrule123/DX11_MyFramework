#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

#include "CStructBuffer.h"

//������� ������Ʈ��
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
    //���� �Ŵ����� ����� ����� ������ ������ ����� �������ص� �ڽ��� ����� ����.
    std::copy(m_vecDebugShapeRender.begin(), m_vecDebugShapeRender.end(), std::back_inserter(_vecDebugRef));
    m_vecDebugShapeRender.clear();
    m_bDebugRenderUpdated = true;
}

void CRenderMgr::init()
{
    //���������� �ȼ������� �ʿ�, 8�� �ؽ�ó �������Ϳ� ���ε� �Ǿ�����.
    m_pLight2DStructBuffer = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE_FLAG::__PIXEL, eSBUFFER_SHARED_CBUFFER_IDX::LIGHT2D, eSRV_REGISTER_IDX::LIGHT2D, eUAV_REGISTER_IDX::NONE);
    m_pLight2DStructBuffer->Create((UINT)sizeof(tLightInfo), 10, nullptr, 0u);
}


void CRenderMgr::render()
{
    UpdateBuffer();

    for (int i = 0; i < (int)eCAMERA_INDEX::END; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //ī�޶󿡼� ������Ʈ�� �����ο� ���� �з��Ѵ�.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }

    //Ŭ���̾�Ʈ �ʿ��� ����׷����� ������ �޾ư��� �ʾ��� ��� ������ ���
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}

void CRenderMgr::UpdateBuffer()
{
    //�۷ι� ������ tGlobalValue ������۷� ������Ʈ
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_TYPE::GLOBAL);
    pConstBuffer->UploadData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->BindBuffer();

    //�ڽ��� ����ȭ ���� �����͸� ������Ʈ ���� �����͸� Ŭ����
    m_pLight2DStructBuffer->UploadData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->BindBufferSRV();
    m_vecLight2DStruct.clear();
}
