#pragma once

#include "CSingleton.h"

class CCamera;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLETON(CRenderMgr)

private:
	CCamera* m_arrCam[eCAMIDX_END];
	list<tDebugShapeInfo> m_listDebugShapeRender;

public:
	void RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo) { m_listDebugShapeRender.push_back(_tDebugShapeInfo); }

	//CEditorObjMgr에서 사용. 해당 객체의 리스트를 레퍼런스 인자로 전달하면 리스트에 정보를 추가해준 뒤 엔진 내부의 리스트를 전부 제거
	void UpdateDebugShapeRender(list<tDebugShapeInfo>& _listDebugRef);

public:
	void init();
	void render();

};

