#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"


//Scalar Parameter : 이 재질을 사용하는 오브젝트 개별로 달라질 수 있는 요소
//Texture Parameter : 이 재질을 사용하는 오브젝트 모두 공통적으로 사용하는 요소



class CStructBuffer;
class CConstBuffer;

class CMaterial :
    public CRes
{
public:
    CMaterial();
    CMaterial(const CMaterial& _Clone);
    virtual ~CMaterial();

    CLONE(CMaterial)

private:
    Ptr<CGraphicsShader>    m_pShader;

    vector<tMtrlScalarData>         m_vecMtrlScalar;
    CConstBuffer*                   m_CBufferMtrlScalar;
    CStructBuffer*                  m_SBufferMtrlScalar;

    tMtrlTexData            m_MtrlTex;

    Ptr<CTexture>           m_arrTex[(int)eMTRLDATA_PARAM_TEX::_END];
    
    //1 = 일반 드로우콜, 2 이상 = 인스턴싱
    //vector은 데이터를 구조화버퍼를 전달하면 싹 지워버리기 문에
    //별도의 변수를 둬서 사이즈를 따로 저장해 놔야 함
    UINT                    m_uRenderCount;

public:
    //Inline Setter
    void AddMtrlScalarData(const tMtrlScalarData& _MtrlScalarData) { m_vecMtrlScalar.push_back(_MtrlScalarData); }
    
    void SetTexParam    (eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex);
    void RemoveTexture  (eMTRLDATA_PARAM_TEX _Param);
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    //Inline Getter
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    Ptr<CTexture> GetTexture(eMTRLDATA_PARAM_TEX _texIdx = eMTRLDATA_PARAM_TEX::_0) const;

    UINT GetInstancingCount() const { return m_uRenderCount; }
    void SetInstancedRender(bool _bEnable);
    bool GetInstencedRender() const { return (nullptr != m_SBufferMtrlScalar); }

private:
    virtual bool Load(const std::filesystem::path& _path) { return true; }
public:
    virtual bool Save(const std::filesystem::path& _path) { return true; }
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
