#pragma once

#include "CSingleton.h"

class CCamera;
class CLight2D;
class CStructBuffer;

class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLETON(CRenderMgr)

private:
	CCamera* m_arrCam[eCAMIDX_END];

	vector<tDebugShapeInfo> m_vecDebugShapeRender;
	//Ŭ���̾�Ʈ ������Ʈ���� �� ����� ������ ������ �޾ư����� Ȯ���ϴ� ����
	//���� Ŭ���̾�Ʈ���� ������ �޾ư��� ���� ��� ����� ������ �迭�� �� �����Ӹ��� �����ǹǷ� �ݵ�� ������ �Ѵ�.
	bool					m_bDebugRenderUpdated;	

	//�� ���� ����ȭ ���� ����
	vector<tLightInfo>		m_vecLight2DStruct;
	CStructBuffer* m_pLight2DStructBuffer;
	

public:
	//inline getter
	CCamera* GetCamera(eCAMERA_INDEX _iCamIdx) { return m_arrCam[_iCamIdx]; }

	void RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx);
	void RemoveCamera(CCamera* _pCam);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo) { m_vecDebugShapeRender.push_back(_tDebugShapeInfo); }

	//CEditorObjMgr���� ���. �ش� ��ü�� ����Ʈ�� ���۷��� ���ڷ� �����ϸ� ����Ʈ�� ������ �߰����� �� ���� ������ ����Ʈ�� ���� ����
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

	void AddLight2DData(const tLightInfo& _tLightInfo) { m_vecLight2DStruct.push_back(_tLightInfo); }


public:
	void init();
	void render();

private:
	void UpdateData();
};

