#pragma once
#include "CShader.h"

//컴퓨트쉐이더는 MtrlConst 상수버퍼를 같이 사용한다.
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    CComputeShader(UINT _uThreadsX, UINT _uThreadsY, UINT _uThreadsZ);
    virtual ~CComputeShader();
    CLONE_DISABLE(CComputeShader);

public:
    virtual bool Load(const std::filesystem::path& _fileName) final;

private://미사용
    virtual void BindData() final {};



protected://아래의 두 함수만 재정의해서 사용해주면 됨.
    //BindDataCS에서 true를 반환 시에만 나머지 과정이 진행됨.
    virtual bool BindDataCS() = 0;
    virtual void UnBindCS() = 0;



protected://아래의 값은 자식 클래스에서 직접 수정
    enum ThreadAxis { X, Y, Z, NumAxis };

private:
    //쉐이더 컴파일 관련
    tShaderLoadData             m_ShaderData;
    ComPtr<ID3D11ComputeShader> m_CS;

    //공유 데이터를 전달하기위한 상수버퍼용 구조체
    tMtrlScalarData                  m_CBuffer_CSShared;

    //그룹당 쓰레드 갯수. 생성자에서 초기화
    UINT                        m_uNumThreadsPerGroupX;
    UINT                        m_uNumThreadsPerGroupY;
    UINT                        m_uNumThreadsPerGroupZ;

    //그룹 갯수.  쓰레드 갯수를 통해서 계산
    UINT                        m_uNumGroupX;
    UINT                        m_uNumGroupY;
    UINT                        m_uNumGroupZ;
    
public:
    void CreateShader(char* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADER_LOADTYPE _eLoadType);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName);

    void CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ);

    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);

    //컴퓨터쉐이더 연산 시행
    bool Execute();
};

