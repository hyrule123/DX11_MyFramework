#pragma once

#include "CSingleton.h"

#include "Shader_STD2D_Light_0_Header.hlsli"

class CCamera;
class CLight2D;
class CStructBuffer;
class CRenderComponent;

//2D���� �� �ӽ� ����ü
//2D������ ���� ��Ʈ������ ��-���� ��Ʈ������ ������ ������ ������
//CPU�� �۾� �д��
struct tRenderInfo
{
	CRenderComponent* pRenderCom;
	CCamera* pCam;
};


class CRenderMgr : public CSingleton<CRenderMgr>
{
	SINGLETON(CRenderMgr)

public:
	void init();
	void tick();
	void render();

private:
	CCamera* m_arrCam[(UINT)eCAMERA_INDEX::END];

	vector<tDebugShapeInfo> m_vecDebugShapeRender;
	//Ŭ���̾�Ʈ ������Ʈ���� �� ����� ������ ������ �޾ư����� Ȯ���ϴ� ����
	//���� Ŭ���̾�Ʈ���� ������ �޾ư��� ���� ��� ����� ������ �迭�� �� �����Ӹ��� �����ǹǷ� �ݵ�� ������ �Ѵ�.
	bool					m_bDebugRenderUpdated;	

	//�� ���� ����ȭ ���� ����
	vector<tLightInfo>		m_vecLight2DStruct;
	CStructBuffer* m_pLight2DStructBuffer;

	//�����Ϳ� ī�޶� �ּ�
	CCamera* m_pEditorCam;
	bool m_bEditorCamMode;

	//ī�޶� ���̴� �����ο� ���� �з��� ����� ����
	vector<tRenderInfo>    m_arrvecShaderDomain[(UINT)eSHADER_DOMAIN::_END];

	//Key : Mtrl �ּ�, Valud : Mesh �ּ�
	unordered_map<DWORD_PTR, DWORD_PTR, tLightHashFunc_DWORD_PTR> m_umapInstancing;
	

public:
	//inline getter
	CCamera* GetCamera(eCAMERA_INDEX _iCamIdx) { return m_arrCam[(int)_iCamIdx]; }
	CCamera* GetCurCamera();

	void RegisterCamera(CCamera* _pCam, eCAMERA_INDEX _idx);
	void RemoveCamera(CCamera* _pCam);
	void AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo);

	//CEditorObjMgr���� ���. �ش� ��ü�� ����Ʈ�� ���۷��� ���ڷ� �����ϸ� ����Ʈ�� ������ �߰����� �� ���� ������ ����Ʈ�� ���� ����
	void UpdateDebugShapeRender(vector<tDebugShapeInfo>& _vecDebugRef);

	void AddLight2DData(const tLightInfo& _tLightInfo) { m_vecLight2DStruct.push_back(_tLightInfo); }

	void ChangeRenderCam(bool _bEditorCam) { m_bEditorCamMode = _bEditorCam; }
	void SetEditorCam(CCamera* _pCamera) { m_pEditorCam = _pCamera; }
	bool IsEditorCamMode() const { return m_bEditorCamMode; }


	//������ �ܰ� ����
	void AddRenderQueue(tRenderInfo _pRenderCom, eSHADER_DOMAIN _eShaderDomain);

private:
	//Upload + Bind
	void UpdateBuffer();

	void render_editor();
	void render_play();

	//RenderMgr�� ���� �����͸� �ϰ������� ������
	void renderAll();
};


inline void CRenderMgr::AddDebugShapeRender(const tDebugShapeInfo& _tDebugShapeInfo)
{
	//�÷��� ����϶��� ����� �������� ���������� ����.
	if (false == m_bEditorCamMode)
		return;

	m_vecDebugShapeRender.push_back(_tDebugShapeInfo);
}

inline CCamera* CRenderMgr::GetCurCamera()
{
	if (m_bEditorCamMode)
		return m_pEditorCam;


	return m_arrCam[(int)eCAMERA_INDEX::MAIN];
}


inline void CRenderMgr::AddRenderQueue(tRenderInfo _pRenderCom, eSHADER_DOMAIN _eShaderDomain)
{
	m_arrvecShaderDomain[(int)_eShaderDomain].push_back(_pRenderCom);
}
