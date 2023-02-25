#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

#include "CStructBuffer.h"

//������� ������Ʈ��
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "CKeyMgr.h"

#include "CRenderComponent.h"

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
    //���� �Ŵ����� ����� ����� ������ ������ ����� �������ص� �ڽ��� ����� ����.
    std::copy(m_vecDebugShapeRender.begin(), m_vecDebugShapeRender.end(), std::back_inserter(_vecDebugRef));
    m_vecDebugShapeRender.clear();
    m_bDebugRenderUpdated = true;
}




void CRenderMgr::init()
{
    //���������� �ȼ������� �ʿ�, 8�� �ؽ�ó �������Ϳ� ���ε� �Ǿ�����.
    m_pLight2DStructBuffer = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, eSHADER_PIPELINE_STAGE::__PIXEL, eCBUFFER_SBUFFER_SHAREDATA_IDX::LIGHT2D, e_t_SBUFFER_LIGHT2D, e_u_UAV_NONE);
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


    //Ŭ���̾�Ʈ �ʿ��� ����׷����� ������ �޾ư��� �ʾ��� ��� ������ ���
    if (false == m_bDebugRenderUpdated)
        m_vecDebugShapeRender.clear();

    m_bDebugRenderUpdated = false;
}


void CRenderMgr::UpdateBuffer()
{
    //�۷ι� ������ tGlobalValue ������۷� ������Ʈ
    CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(e_b_CBUFFER_GLOBAL);
    pConstBuffer->UploadData((void*)(&g_GlobalVal), sizeof(tGlobalValue));
    pConstBuffer->BindBuffer();

    //�ڽ��� ����ȭ ���� �����͸� ������Ʈ ���� �����͸� Ŭ����
    m_pLight2DStructBuffer->UploadData(static_cast<void*>(m_vecLight2DStruct.data()), (UINT)m_vecLight2DStruct.size());
    m_pLight2DStructBuffer->BindBufferSRV();
    m_vecLight2DStruct.clear();
}

void CRenderMgr::render_editor()
{
    assert(nullptr != m_pEditorCam);

    m_pEditorCam->SortObject();

    renderAll();
}

void CRenderMgr::render_play()
{
    for (int i = 0; i < (int)eCAMERA_INDEX::END; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //ī�޶󿡼� ������Ʈ�� �����ο� ���� �з��Ѵ�.
        m_arrCam[i]->SortObject();
    }

    renderAll();
}

void CRenderMgr::renderAll()
{
    //2D ���� ���ȿ��� W�� VP�� ���̴����� ����ϰ�,
    //3D ������ �� ���ĺ��� WVP�� �ϰ������� �Ѱ��ֵ��� ������ ����,
    //�׷��� ������ ī�޶� �������� �ν��Ͻ� �� �������� ������
    // 
    //2D���� �޽��� ������ ��κ� 4���̹Ƿ� GPU�� �۾��� �� �д��Ϸ��� ��
    //���߿� 3D ���� ���� �ڵ带 ������ ��
    for (UINT i = 0; i < (UINT)eSHADER_DOMAIN::_END; ++i)
    {
        CCamera* pPrevCam = nullptr;
        size_t size = m_arrvecShaderDomain[i].size();
        for (size_t j = 0; j < size; j++)
        {
            if (pPrevCam != m_arrvecShaderDomain[i][j].pCam)
            {

                //ù ���� ī�޶� ��ü�� �ƴ� ���(������ ��ϵ� ī�޶� �ִµ�, �ٸ� ī�޶�� ��ü�ϴ� ��Ȳ�� ���)
                //�Ǵ� �ݺ����� ���� �������� ���
                //������ �ѹ� ����(2D ����)
                if (nullptr != pPrevCam || j + 1 == size)
                {
                    //Ȥ�ó� �׸��� �� �Ѱ��ϰ�쿡�� ī�޶� �����͸� ���ε��� �����
                    if (nullptr == pPrevCam)
                        m_arrvecShaderDomain[i][j].pCam->UploadData();

                    //�ν��Ͻ� ������ ����(ī�޶� ����� ��ϵǾ� ����)
                    for (const auto& iter : m_umapInstancing)
                    {
                        CMaterial* pMtrl = (CMaterial*)(iter.first);
                        pMtrl->BindData();
                        ((CMesh*)(iter.second))->renderInstanced(pMtrl->GetInstancingCount());
                    }

                    //�ν��Ͻ� ��⿭ Ŭ����
                    m_umapInstancing.clear();
                }

                //ī�޶� �������� ��ü�ϰ� �� ����� ���ε�(2D���� ���� - 3D ���� ���� ����)
                m_arrvecShaderDomain[i][j].pCam->UploadData();
                pPrevCam = m_arrvecShaderDomain[i][j].pCam;
            }


            //���� render �޼ҵ带 ȣ���ߴµ� ��ο����� �߻����� �ʾҴٸ�(==�ν��Ͻ����� �׸��ڴٰ� �����Ǿ� ������)
            if (false == m_arrvecShaderDomain[i][j].pRenderCom->render())
            {
                //�ν��Ͻ� ��⿭ map�� �߰�
                DWORD_PTR pMtrl = (DWORD_PTR)(m_arrvecShaderDomain[i][j].pRenderCom->GetCurMaterial().Get());

                //�ߺ��߰� ����
                if (m_umapInstancing.end() == m_umapInstancing.find(pMtrl))
                {
                    m_umapInstancing[pMtrl] = (DWORD_PTR)(m_arrvecShaderDomain[i][j].pRenderCom->GetMesh().Get());
                }
            }
        }

        //�������� ���̴� �������� ����
        m_arrvecShaderDomain[i].clear();
    }
}
