#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ);
    virtual ~CComputeShader();
    CLONE_DISABLE(CComputeShader);

protected:
    virtual void BindData() = 0;
    virtual void UnBind() = 0;

private:
    tShaderLoadData             m_ShaderData;

    ComPtr<ID3D11ComputeShader> m_CS;


protected://아래의 값은 자식 클래스에서 직접 수정
    enum ThreadAxis
    {
        X,
        Y,
        Z,
        NumAxis
    };
    
    UINT                        m_arrGroup[NumAxis];
    UINT                        m_arrThreadsPerGroup[NumAxis];


    //공유 데이터를 전달하기위한 상수버퍼용 구조체
    tMtrlConst                  m_SharedCBuffer;

public:
    void CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName);

    tMtrlConst& GetSharedCBuffer() { return m_SharedCBuffer; }

    //컴퓨터쉐이더 연산 시행
    void Execute();
};

