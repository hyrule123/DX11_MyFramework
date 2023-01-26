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
    //���� �Ŵ����� ����� ����� ������ ������ ����� �������ص� �ڽ��� ����� ����.
    std::copy(m_vecDebugShapeRender.begin(), m_vecDebugShapeRender.end(), std::back_inserter(_vecDebugRef));
    m_vecDebugShapeRender.clear();
    m_bDebugRenderUpdated = true;
}

void CRenderMgr::init()
{
    //���������� �ȼ������� �ʿ�, 8�� �ؽ�ó �������Ϳ� ���ε� �Ǿ�����.
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

        //ī�޶󿡼� ������Ʈ�� �����ο� ���� �з��Ѵ�.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }

    //Ŭ���̾�Ʈ �ʿ��� ����׷����� ������ �޾ư��� �ʾ��� ��� ������ ���
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}

void CRenderMgr::UpdateData()
{
    //�۷ι� ������ tGlobalValue ������۷� ������Ʈ
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(eCONST_BUFFER_GLOBAL);
    
    g_GlobalVal.Light2DCount = (UINT)m_vecLight2DStruct.size();
    g_GlobalVal.Light3DCount = 0u;

    pConstBuffer->SetData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->UpdateData();


    //�ڽ��� ����ȭ ���� �����͸� ������Ʈ ���� �����͸� Ŭ����
    m_pLight2DStructBuffer->SetData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->UpdateData();
    m_vecLight2DStruct.clear();
}
