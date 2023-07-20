#pragma once
#include "IShader.h"

//=============사용법============
//컴퓨트쉐이더의 HLSL 코드는 C++ cComputeShader 클래스에 종속됨
//하나의 HLSL CS = 하나의 cComputeShader 클래스
//그러므로 상속받아서 만드는 생성자에서 컴퓨트쉐이더를 로드해주면 된다.

namespace JsonKey_cComputeShader
{
    //쉐이더와 json 파일이 공유하는 파일 이름. 확장자 제외
    //ex) S_C_SCMapLoader.json : S_C_SCMapLoader
    STRKEY_DECLARE(m_arrNumThreads);
    STRKEY_DECLARE(ShaderModule);
}

class cShaderDataModule;
class cComputeShader :
    public IShader
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
    virtual bool BindData() final;


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
    UINT               m_arrNumThreads[NumAxis];
    tComputeShaderInfo m_ComputeShaderInfo;
public:
    //스레드 갯수 계산
    void SetThreadsPerGroup(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ);

    //그룹 갯수 계산
    void CalcGroupNumber(const tNumDataCS& _NumData);

    //쉐이더에 추가적으로 필요할 경우 붙일 수 있는 데이터 클래스(모듈)
private:
    cShaderDataModule* m_pShaderDataModule;
public:
    //데이터 소멸을 여기서 관리하지 않으므로 주의할 것
    void SetShaderDataModule(cShaderDataModule* _pShaderModule) { m_pShaderDataModule = _pShaderModule; }
    cShaderDataModule* GetShaderDataModule() { return m_pShaderDataModule; }

public:
    //컴퓨터쉐이더 연산 시행
    bool Execute();

private:
    inline bool IsDataReady();
    inline bool IsDataModuleReady() { return m_pShaderDataModule; }
    inline bool IsReady() { return (nullptr != m_CS) && IsDataReady() && IsDataModuleReady(); }

};

inline HRESULT cComputeShader::CreateShader(ComPtr<ID3DBlob> _pBlob)
{
    m_pShaderData.Swap(_pBlob);
    return CreateShaderPrivate(m_pShaderData->GetBufferPointer(), m_pShaderData->GetBufferSize());
}

inline void cComputeShader::SetThreadsPerGroup(UINT _uThreadX, UINT _uThreadY, UINT _uThreadZ)
{
    assert(_uThreadX && _uThreadY && _uThreadZ);
    m_arrNumThreads[X] = _uThreadX;
    m_arrNumThreads[Y] = _uThreadY;
    m_arrNumThreads[Z] = _uThreadZ;
}
