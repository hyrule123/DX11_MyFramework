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

	//CEditorObjMgr���� ���. �ش� ��ü�� ����Ʈ�� ���۷��� ���ڷ� �����ϸ� ����Ʈ�� ������ �߰����� �� ���� ������ ����Ʈ�� ���� ����
	void UpdateDebugShapeRender(list<tDebugShapeInfo>& _listDebugRef);

public:
	void init();
	void render();

};

