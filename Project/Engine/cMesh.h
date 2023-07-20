#pragma once
#include "IRes.h"

class cMesh
	: public IRes
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_tVBDesc;
	UINT					m_VtxCount;

	ComPtr<ID3D11Buffer>	m_IB;
	D3D11_BUFFER_DESC		m_tIBDesc;
	UINT					m_IdxCount;

	void*					m_pVtxSys;
	void*					m_pIdxSys;

public:
	//이거 가지고 W / VP 행렬을 나눠서 보낼지, WVP 행렬을 만들어서 보낼지를 결정
	UINT GetIdxBufferCount() const { return m_IdxCount; }

	void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);

private:
	//virtual bool Load(const std::filesystem::path& _fileName) { return true; }
public:
	//virtual bool Save(const std::filesystem::path& _fileName) { return true; }


	void Render();
	void renderInstanced(UINT _uInstancingCount);


private:
	bool BindData();
	

public:
	cMesh();
	~cMesh();
};
