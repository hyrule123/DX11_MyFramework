#pragma once
#include "CShader.h"

//��ǻƮ���̴��� MtrlConst ������۸� ���� ����Ѵ�.
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ);
    virtual ~CComputeShader();
    CLONE_DISABLE(CComputeShader);

private:
    virtual void BindData() final {};


protected://�Ʒ��� �� �Լ��� �������ؼ� ������ָ� ��.
    //BindDataCS���� true�� ��ȯ �ÿ��� ������ ������ �����.
    virtual bool BindDataCS() = 0;
    virtual void UnBindCS() = 0;

protected://�Ʒ��� ���� �ڽ� Ŭ�������� ���� ����
    enum ThreadAxis { X, Y, Z, NumAxis };

private:
    //���̴� ������ ����
    tShaderLoadData             m_ShaderData;
    ComPtr<ID3D11ComputeShader> m_CS;

    //���� �����͸� �����ϱ����� ������ۿ� ����ü
    tMtrlConst                  m_SharedCBuffer;

    //�׷�� ������ ����. �����ڿ��� �ʱ�ȭ
    UINT                        m_arrThreadsPerGroup[NumAxis];

    //�׷� ����.  ������ ������ ���ؼ� ���
    UINT                        m_arrGroup[NumAxis];
    
public:
    void CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName);

    void CalcGroupNumber(UINT _TotalCountX, UINT _TotalCountY, UINT _TotalCountZ);

    void SetScalarParam(eSCALAR_PARAM _Param, const void* _Src);

    //��ǻ�ͽ��̴� ���� ����
    void Execute();
};

