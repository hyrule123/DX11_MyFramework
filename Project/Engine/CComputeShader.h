#pragma once
#include "CShader.h"

//=============사용법============
//컴퓨트쉐이더의 HLSL 코드는 C++ CComputeShader 클래스에 종속됨
//하나의 HLSL CS = 하나의 CComputeShader 클래스
//그러므로 상속받아서 만드는 생성자에서 컴퓨트쉐이더를 로드해주면 된다.

//컴퓨트쉐이더는 MtrlConst 상수버퍼를 같이 사용한다.
class CComputeShader :
    public CShader
{
public:
    //ComputeShader
    CComputeShader() = delete;

    //CComputeShader를 상속받는 클래스는 생성자에서 '무조건' 스레드 갯수를 설정하고, 컴퓨트쉐이더를 로드해야 함.
    CComputeShader(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ);

    virtual ~CComputeShader();
    CLONE_DISABLE(CComputeShader);

public:
    //기본적으로 Save는 사용하지 않음(대신 .cso 파일 로딩을 위한 Load는 사용)
    //virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool Load(const std::filesystem::path& _fileName) override;
    //virtual bool SaveJson(Json::Value* _jsonVal) override;
    //virtual bool LoadJson(Json::Value* _jsonVal) override;

private://미사용
    virtual void BindData() final {};


protected://아래의 함수만 재정의해서 사용해주면 됨.
    //BindDataCS에서 true를 반환 시에만 나머지 과정이 진행됨.
    virtual bool BindDataCS() = 0;
    virtual void UnBindCS() = 0;



protected://아래의 값은 자식 클래스에서 직접 수정
    enum ThreadAxis { X, Y, Z, NumAxis };

private:
    //쉐이더 컴파일 관련
    //Blob = Binary Large Object
    ComPtr<ID3DBlob>            m_pShaderData;

    ComPtr<ID3D11ComputeShader> m_CS;

    //공유 데이터를 전달하기위한 상수버퍼용 구조체
    tMtrlScalarData                  m_CBuffer_CSShared;

    //그룹당 쓰레드 갯수. 생성자에서 초기화
    UINT                        m_uNumThreadPerGroupArr[NumAxis];

    //그룹 갯수.  쓰레드 갯수를 통해서 계산
    UINT                        m_uNumGroupArr[NumAxis];
    
public:
    HRESULT CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize);
    HRESULT CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName);
    HRESULT CreateShader(ComPtr<ID3DBlob> _pBlob);
private:
    HRESULT CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize);

public:
    void CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ);

    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);

    //컴퓨터쉐이더 연산 시행
    bool Execute();
};

inline HRESULT CComputeShader::CreateShader(ComPtr<ID3DBlob> _pBlob)
{
    m_pShaderData.Swap(_pBlob);
    return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}


