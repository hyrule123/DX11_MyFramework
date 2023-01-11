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

void CRenderMgr::UpdateDebugShapeRender(list<tDebugShapeInfo>& _listDebugRef)
{
    std::copy(m_listDebugShapeRender.begin(), m_listDebugShapeRender.end(), std::back_inserter(_listDebugRef));
    m_listDebugShapeRender.clear();
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
