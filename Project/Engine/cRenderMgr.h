#pragma once

#include "Singleton.h"

#include "define.h"
#include "struct.h"

#include "S_H_STD2DLight.hlsli"


class cCom_Camera;
class cCom_Light2D;
class cStructBuffer;
class IRenderer;

//2D때만 쓸 임시 구조체
//2D까지는 월드 매트릭스와 뷰-투영 매트릭스를 나눠서 전달할 예정임
//CPU와 작업 분담용
struct tRenderInfo
{
	IRenderer* pRenderCom;
	cCom_Camera* pCam;
};





class cRenderMgr
	: public Singleton<cRenderMgr>
{
	SINGLETON(cRenderMgr);

public:
	void init();
	void tick();
	void render();

private:
	cCom_Camera* m_arrCam[(UINT)eCAMERA_INDEX::END];

	vector<tDebugShapeInfo> m_vecDebugShapeRender;
	//클라이언트 프로젝트에서 위 디버그 렌더링 정보를 받아갔는지 확인하는 변수
	//만약 클라이언트에서 정보를 받아가지 않을 경우 디버그 렌더링 배열이 매 프레임마다 누적되므로 반드시 비워줘야 한다.
	bool					m_bDebugRenderUpdated;	

	//빛 관련 구조화 버퍼 변수
	vector<tLightInfo>		m_veccLight2DStruct;
	std::unique_ptr<cStructBuffer>			m_pcLight2DStructBuffer;

	//에디터 카메라 모드인지를 저장하는 변수(Ctrl + E)버튼으로 확인 중
	bool m_bEditorCamMode;

	//카메라가 쉐이더 도메인에 따라 분류한 결과를 저장
	vector<tRenderInfo>    m_arrvecShaderDomain[(UINT)define_Shader::eSHADER_DOMAIN::_END];

	//Key: cMesh + Mtrl 주소 담긴 구조체, Value: 동일 조건으로 인스턴싱 될 객체의 정보 vector
	map<tInstancingKey, vector<tMtrlScalarData>> m_mapInstancing;

	//인스턴싱용 SBuffer
	std::unique_ptr<cStructBuffer> m_pSBuffer_Instancing;

public:
	//inline getter
	cCom_Camera* GetCamera(eCAMERA_INDEX _iCamIdx) { return m_arrCam[(int)_iCamIdx]; }
	cCom_Camera* GetCurCamera();

	void RegisterCamera(cCom_Camera* _pCam, eCAMERA_INDEX _idx);
	void RemoveCamera(cCom_Camera* _pCam);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo);

	//CEditorObjMgr에서 사용. 해당 객체의 리스트를 레퍼런스 인자로 전달하면 리스트에 정보를 추가해준 뒤 엔진 내부의 리스트를 전부 제거
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

	void AddcLight2DData(const tLightInfo& _tLightInfo) { m_veccLight2DStruct.push_back(_tLightInfo); }

	void ChangeRenderCam(bool _bEditorCam) { m_bEditorCamMode = _bEditorCam; }
	void SetEditorCam(cCom_Camera* _pCamera) { m_arrCam[(int)eCAMERA_INDEX::EDITOR] = _pCamera; }
	bool IsEditorCamMode() const { return m_bEditorCamMode; }

	//렌더링 단계 관련
	void AddRenderQueue(tRenderInfo _pRenderCom, define_Shader::eSHADER_DOMAIN _eShaderDomain);

	void AddInstancingQueue(const tInstancingKey& _Key, const tMtrlScalarData& _Value);
	void InstancedRender();

private:
	//Upload + Bind
	void UpdateBuffer();

	void render_editor();
	void render_play();

	//cRenderMgr에 모인 데이터를 일괄적으로 렌더링
	void renderAll();
	
};


inline void cRenderMgr::AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo)
{
	//플레이 모드일때는 디버그 쉐이프를 렌더링하지 않음.
	if (false == m_bEditorCamMode)
		return;

	m_vecDebugShapeRender.push_back(_tDebugShapeInfo);
}

inline cCom_Camera* cRenderMgr::GetCurCamera()
{
	if (m_bEditorCamMode)
		return m_arrCam[(int)eCAMERA_INDEX::EDITOR];


	return m_arrCam[(int)eCAMERA_INDEX::MAIN];
}


inline void cRenderMgr::AddRenderQueue(tRenderInfo _pRenderCom, define_Shader::eSHADER_DOMAIN _eShaderDomain)
{
	m_arrvecShaderDomain[(int)_eShaderDomain].push_back(_pRenderCom);
}
