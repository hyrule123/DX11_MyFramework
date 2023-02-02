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


protected://�Ʒ��� ���� �ڽ� Ŭ�������� ���� ����
    enum ThreadAxis
    {
        X,
        Y,
        Z,
        NumAxis
    };
    
    UINT                        m_arrGroup[NumAxis];
    UINT                        m_arrThreadsPerGroup[NumAxis];


    //���� �����͸� �����ϱ����� ������ۿ� ����ü
    tMtrlConst                  m_SharedCBuffer;

public:
    void CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName);

    tMtrlConst& GetSharedCBuffer() { return m_SharedCBuffer; }

    //��ǻ�ͽ��̴� ���� ����
    void Execute();
};

