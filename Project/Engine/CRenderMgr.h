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
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo) { m_vecDebugShapeRender.push_back(_tDebugShapeInfo); }

	//CEditorObjMgr���� ���. �ش� ��ü�� ����Ʈ�� ���۷��� ���ڷ� �����ϸ� ����Ʈ�� ������ �߰����� �� ���� ������ ����Ʈ�� ���� ����
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

public:
	void init();
	void render();

};

