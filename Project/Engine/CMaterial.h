#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"


//Scalar Parameter : 이 재질을 사용하는 오브젝트 개별로 달라질 수 있는 요소
//Texture Parameter : 이 재질을 사용하는 오브젝트 모두 공통적으로 사용하는 요소
//  그러므로 만약 Texture Parameter를 다르게 사용하고 싶을 경우 Material을 복사해서 렌더링해야 한다.

class CStructBuffer;
class CConstBuffer;

class CMaterial :
    public CRes
{
public:
    CMaterial();
    virtual ~CMaterial();

    CMaterial(const CMaterial& _Clone);
    CLONE(CMaterial)
public:
    virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _pJson) override;
    virtual bool LoadJson(Json::Value* _pJson) override;

public:
    virtual void BindData() override;

private:
    Ptr<CGraphicsShader>    m_pShader;
    
    Ptr<CTexture>           m_arrTex[(int)eMTRLDATA_PARAM_TEX::_END];
    tMtrlTexData            m_MtrlTex;

    //복사가 일어났을 때 이 값은 true로 변함
    const bool              m_bIsDynamic;

public:
    void SetTexParam    (eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex);
    void RemoveTexture  (eMTRLDATA_PARAM_TEX _Param);
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    //쉐이더의 인스턴싱 가능 여부를 받아온다.
    bool IsUseInstancing() const { if (nullptr != m_pShader) return m_pShader->IsUseInstancing(); return false; }

    //Inline Getter
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    Ptr<CTexture> GetTexture(eMTRLDATA_PARAM_TEX _texIdx = eMTRLDATA_PARAM_TEX::_0) const;

    //void SetInstancingMode(bool _bUse);
    //bool GetInstancingMode() const { return m_bUseInstancing; }

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
