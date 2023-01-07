#pragma once

#include "CSingleton.h"

class CCamera;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLETON(CRenderMgr)

private:
	CCamera* m_arrCam[eCAMIDX_END];

public:
	bool RegisterCamera(CCamera* _pCam, UINT _idx);

public:
	void init();
	void render();

};

