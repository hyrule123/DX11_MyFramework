#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"


//Scalar Parameter : 이 재질을 사용하는 오브젝트 개별로 달라질 수 있는 요소
//Texture Parameter : 이 재질을 사용하는 오브젝트 모두 공통적으로 사용하는 요소


struct tMapEntityIDHashFunc
{
    size_t operator()(const UINT32& _ukey) const
    {
        return static_cast<size_t>(_ukey);
    }
};

class CStructBuffer;

class CMaterial :
    public CRes
{
private:
    CMaterial() = delete;
public:
    CMaterial(bool _bUseInstancing);
    CMaterial(const CMaterial& _Clone);
    virtual ~CMaterial();

    CLONE(CMaterial)

private:
    Ptr<CGraphicsShader>    m_pShader;

    //Key : 자신의 entity ID 정보, Value : 자신의 정보가 저장되어있는 m_vecMtrlScalar에서의 index 값
    //재질을 관리하는 컴포넌트인 RenderComponent의 소멸자에서 반드시 제거해주어야 함.
    unordered_map<UINT32, UINT, tMapEntityIDHashFunc>      m_mapEntityID;
    vector<tMtrlScalarData>         m_vecMtrlScalar;
    CStructBuffer*                  m_SBufferMtrlScalar;

    tMtrlTexData            m_MtrlTex;

    Ptr<CTexture>           m_arrTex[(int)eMTRLDATA_PARAM_TEX::_END];

    //이번 프레임 인스턴싱으로 그리는지 일반 드로우콜로 그리는지 전달
    UINT                    m_uCurFrmInstancingCount;
    

public:
    //Inline Setter
    void SetScalarParam (UINT32 _iRenderComEntityID, eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);
    void SetTexParam    (eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex);
    void RemoveTexture  (eMTRLDATA_PARAM_TEX _Param);
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    //Inline Getter
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    Ptr<CTexture> GetTexture(eMTRLDATA_PARAM_TEX _texIdx = eMTRLDATA_PARAM_TEX::_0) const;

    UINT GetInstancingCount() const { return m_uCurFrmInstancingCount; }

private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }
    virtual void BindData() override;

private:
    bool IsTexture(eMTRLDATA_PARAM_TEX _Idx);

};

inline Ptr<CTexture> CMaterial::GetTexture(eMTRLDATA_PARAM_TEX _texIdx) const
{
    return m_arrTex[(UINT)_texIdx];
}

inline bool CMaterial::IsTexture(eMTRLDATA_PARAM_TEX _Idx)
{
    switch (_Idx)
    {
    case eMTRLDATA_PARAM_TEX::_0: return (bool)m_MtrlTex.bTEX_0;
    case eMTRLDATA_PARAM_TEX::_1: return (bool)m_MtrlTex.bTEX_1;
    case eMTRLDATA_PARAM_TEX::_2: return (bool)m_MtrlTex.bTEX_2;
    case eMTRLDATA_PARAM_TEX::_3: return (bool)m_MtrlTex.bTEX_3;
    case eMTRLDATA_PARAM_TEX::_4: return (bool)m_MtrlTex.bTEX_4;
    case eMTRLDATA_PARAM_TEX::_5: return (bool)m_MtrlTex.bTEX_5;
    case eMTRLDATA_PARAM_TEX::_6: return (bool)m_MtrlTex.bTEX_6;
    case eMTRLDATA_PARAM_TEX::_7: return (bool)m_MtrlTex.bTEX_7;
    case eMTRLDATA_PARAM_TEX::_END: break;
    default: break;
    }

    return false;
}

inline void CMaterial::RemoveTexture(eMTRLDATA_PARAM_TEX _Param)
{
    m_arrTex[(int)_Param] = nullptr;

    switch (_Param)
    {
    case eMTRLDATA_PARAM_TEX::_0: m_MtrlTex.bTEX_0 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_1: m_MtrlTex.bTEX_1 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_2: m_MtrlTex.bTEX_2 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_3: m_MtrlTex.bTEX_3 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_4: m_MtrlTex.bTEX_4 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_5: m_MtrlTex.bTEX_5 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_6: m_MtrlTex.bTEX_6 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_7: m_MtrlTex.bTEX_7 = FALSE; break;
    case eMTRLDATA_PARAM_TEX::_END: break;
    default: break;
    }
}
