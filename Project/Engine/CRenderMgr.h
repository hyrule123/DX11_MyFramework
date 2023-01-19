#pragma once

#include "CSingleton.h"

class CCamera;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLETON(CRenderMgr)

private:
	CCamera* m_arrCam[eCAMIDX_END];
	vector<tDebugShapeInfo> m_vecDebugShapeRender;

public:
	//inline getter
	CCamera* GetCamera(eCAMERA_INDEX _iCamIdx) { return m_arrCam[_iCamIdx]; }

	void RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx);
	void RemoveCamera(CCamera* _pCam);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo) { m_vecDebugShapeRender.push_back(_tDebugShapeInfo); }

	//CEditorObjMgr에서 사용. 해당 객체의 리스트를 레퍼런스 인자로 전달하면 리스트에 정보를 추가해준 뒤 엔진 내부의 리스트를 전부 제거
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

public:
	void init();
	void render();

};

