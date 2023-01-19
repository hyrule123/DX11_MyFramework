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

        //카메라에서 오브젝트를 도메인에 따라서 분류한다.
        m_arrCam[i]->SortObject();
        m_arrCam[i]->render();
    }
}
