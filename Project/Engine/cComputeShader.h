#pragma once
#include "CShader.h"

//=============사용법============
//컴퓨트쉐이더의 HLSL 코드는 C++ cComputeShader 클래스에 종속됨
//하나의 HLSL CS = 하나의 cComputeShader 클래스
//그러므로 상속받아서 만드는 생성자에서 컴퓨트쉐이더를 로드해주면 된다.

class cShderModule;
class cComputeShader :
    public CShader
{
public:
    //ComputeShader
    cComputeShader();

    virtual ~cComputeShader();
    CLONE_DISABLE(cComputeShader);

public:
    //virtual bool Save(const std::filesystem::path& _fileName) override;
    //virtual bool Load(const std::filesystem::path& _fileName) override;
    virtual bool SaveJson(Json::Value* _jsonVal) override;
    virtual bool LoadJson(Json::Value* _jsonVal) override;

private:
    virtual void BindData() final;


protected://아래의 값은 자식 클래스에서 직접 수정
    enum ThreadAxis { X, Y, Z, NumAxis };

private:
    //쉐이더 컴파일 관련
    //Blob = Binary Large Object
    ComPtr<ID3DBlob>            m_pShaderData;
    ComPtr<ID3D11ComputeShader> m_CS;

public:
    HRESULT CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize);
    HRESULT CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName);
    HRESULT CreateShader(ComPtr<ID3DBlob> _pBlob);
private:
    HRESULT CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize);

private:
    //공유 데이터를 전달하기위한 상수버퍼용 구조체
    tMtrlScalarData                  m_CBuffer_CSShared;

    //그룹당 쓰레드 갯수. 생성자에서 초기화
    UINT                        m_arrNumThread[NumAxis];
public:
    void SetThreadNumber(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ);


private:
    //그룹 갯수.  쓰레드 갯수를 통해서 계산
    UINT                        m_uNumGroupArr[NumAxis];
public:
    void CalcGroupNumber(UINT _ElemCountX, UINT _ElemCountY, UINT _ElemCountZ);

    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);


    //쉐이더에 추가적으로 필요할 경우 붙일 수 있는 데이터 클래스(모듈)
private:
    std::shared_ptr<cShderModule> m_pShaderModule;
public:
    bool AddShaderModule(std::unique_ptr<cShderModule>&& _pShaderModule);
    cShderModule* GetShaderModule() { return m_pShaderModule.get(); }

public:
    //컴퓨터쉐이더 연산 시행
    bool Execute();
};

inline HRESULT cComputeShader::CreateShader(ComPtr<ID3DBlob> _pBlob)
{
    m_pShaderData.Swap(_pBlob);
    return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}

inline void cComputeShader::SetThreadNumber(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ)
{
    assert(_uThreadX && _uThreadY && _uThreadZ);
    m_arrNumThread[X] = _uThreadX;
    m_arrNumThread[Y] = _uThreadY;
    m_arrNumThread[Z] = _uThreadZ;
}
