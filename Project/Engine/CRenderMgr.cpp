#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

CRenderMgr::CRenderMgr():
    m_arrCam{}
{

}

CRenderMgr::~CRenderMgr()
{

}


bool CRenderMgr::RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx)
{
    if (_idx >= eCAMIDX_END)
        return false;

    m_arrCam[_idx] = _pCam;
        
    return true;
}

void CRenderMgr::init()
{
}

void CRenderMgr::render()
{
    for (int i = 0; i < eCAMIDX_END; ++i)
    {
        if (nullptr == m_arrCam[i])
            continue;

        //ī�޶󿡼� ������Ʈ�� �����ο� ���� �з��Ѵ�.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }
}
