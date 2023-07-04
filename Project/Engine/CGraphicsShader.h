#pragma once
#include "CShader.h"

#include "global.h"

//참고 - CEntity의 'Name' 변수에 쉐이더의 BaseName을 저장함.
struct tShaderCode
{
    ComPtr<ID3DBlob> blob;
    string strKey;
};

class CGraphicsShader :
    public CShader
{
public:
    CGraphicsShader();
    virtual ~CGraphicsShader();

public:
    virtual bool SaveJson(Json::Value* _jsonVal) override;
    virtual bool LoadJson(Json::Value* _jsonVal) override;

private:
    //지정된 각 쉐이더별로 어떤 방식으로 로드했는지를 저장.
    tShaderCode                     m_arrShaderCode[(int)define_Shader::eGS_TYPE::END];

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_InputLayout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;


    //Rasterizer
    define_Shader::eRASTERIZER_TYPE                m_eRSType;       //Rasterizer Type
    
    //Output Merger
    ////Depth & Stencil
    define_Shader::eDEPTH_STENCIL_TYPE              m_eDSType;      //DepthStencil Type
    ////Blend State
    define_Shader::eBLEND_STATE_TYPE                m_eBSType;

    define_Shader::eSHADER_DOMAIN                  m_eShaderDomain;

    bool                            m_bUseInstancing;

private:


public:
    //Compile from raw source file
    //런타임에 컴파일해서 쉐이더를 만드는 방식에 사용
    //cso파일과 마찬가지로 Contents/Shader/Graphics/ 아래에 저장할것
    HRESULT CreateShader(const std::filesystem::path& _FileName, const string_view _strFuncName, define_Shader::eGS_TYPE _ShaderType);

    //Compile from Header(헤더 파일의 경우 이미 데이터가 프로그램에 삽입되므로 복사 필요 x)
    //엔진 내부 기본 쉐이더들을 위한 함수
    //엔진 내부 기본 쉐이더와 직접 만든 쉐이더를 동시에 사용 불가.
    HRESULT CreateShaderFromHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize, define_Shader::eGS_TYPE _eShaderType);

    //Compile from Byte Code inside of blob(cso)
    //바이트 코드의 경우 메모리 릭 방지를 위해 ComPtr<ID3DBlob>에 넣어서 전달할 것
    HRESULT CreateShader(const tShaderCode& _tShaderCode, define_Shader::eGS_TYPE _ShaderType);

    
private:
    //Vertex Buffer 생성 후 입력조립기 생성
    HRESULT CreateDefaultInputLayout();

    //최종 내부 쉐이더 컴파일 코드
    HRESULT CreateShaderPrivate(const void* _pByteCode, size_t _ByteCodeSize, define_Shader::eGS_TYPE _ShaderType);

public:

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRasterizerState(define_Shader::eRASTERIZER_TYPE _eRS_TYPE) { m_eRSType = _eRS_TYPE; }

    void SetDepthStencilState(define_Shader::eDEPTH_STENCIL_TYPE _eDS_TYPE) { m_eDSType = _eDS_TYPE; }

    void SetBlendState(define_Shader::eBLEND_STATE_TYPE _eBS_TYPE) { m_eBSType = _eBS_TYPE; }

    void SetShaderDomain(define_Shader::eSHADER_DOMAIN _eSD_TYPE) { m_eShaderDomain = _eSD_TYPE; }
    define_Shader::eSHADER_DOMAIN GetShaderDomain() const { return m_eShaderDomain; }

    void SetUseInstancing(bool _bAble) { m_bUseInstancing = _bAble; }
    bool IsUseInstancing() const { return m_bUseInstancing; }


    

public:
    virtual void BindData() override;
};

inline HRESULT CGraphicsShader::CreateShader(const tShaderCode& _tShaderCode, define_Shader::eGS_TYPE _eShaderType)
{
    m_arrShaderCode[(int)_eShaderType] = _tShaderCode;

    //쉐이더를 생성한다.
    return CreateShaderPrivate(_tShaderCode.blob->GetBufferPointer(), _tShaderCode.blob->GetBufferSize(), _eShaderType);
}


