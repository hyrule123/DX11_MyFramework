#pragma once
#include "C_ComputeShader.h"

#include "Ptr.h"



class Texture;
class cStructBuffer;

class CCS_SetColor :
    public C_ComputeShader
{
public:
    CCS_SetColor();
    virtual ~CCS_SetColor();

public:
    virtual bool Load(std::filesystem::path const& _FilePath) override;

protected:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    Ptr<Texture>   m_OutTex;
    std::unique_ptr<cStructBuffer>   m_StructBufferTest;
    Vec4            m_vecSBuffer[1280];

public:
    void SetTargetTexture(Ptr<Texture> _Tex) { m_OutTex = _Tex; }
    void SetColor(const Vec3& _RGB) { SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::VEC4_0, &_RGB); }
};
