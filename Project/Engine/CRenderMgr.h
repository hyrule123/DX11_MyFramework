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
	//클라이언트 프로젝트에서 위 디버그 렌더링 정보를 받아갔는지 확인하는 변수
	//만약 클라이언트에서 정보를 받아가지 않을 경우 디버그 렌더링 배열이 매 프레임마다 누적되므로 반드시 비워줘야 한다.
	bool					m_bDebugRenderUpdated;	

	//빛 관련 구조화 버퍼 변수
	vector<tLightInfo>		m_vecLight2DStruct;
	CStructBuffer* m_pLight2DStructBuffer;
	

public:
	//inline getter
	CCamera* GetCamera(eCAMERA_INDEX _iCamIdx) { return m_arrCam[_iCamIdx]; }

	void RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx);
	void RemoveCamera(CCamera* _pCam);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo) { m_vecDebugShapeRender.push_back(_tDebugShapeInfo); }

	//CEditorObjMgr에서 사용. 해당 객체의 리스트를 레퍼런스 인자로 전달하면 리스트에 정보를 추가해준 뒤 엔진 내부의 리스트를 전부 제거
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

	void AddLight2DData(const tLightInfo& _tLightInfo) { m_vecLight2DStruct.push_back(_tLightInfo); }


public:
	void init();
	void render();

private:
	void UpdateData();
};

