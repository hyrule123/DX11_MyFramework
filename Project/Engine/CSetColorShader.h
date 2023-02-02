#pragma once
#include "CComputeShader.h"

#include "ptr.h"

class CTexture;
class CStructBuffer;

class CSetColorShader :
    public CComputeShader
{
public:
    CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    virtual ~CSetColorShader();

protected:
    virtual void BindData() override;
    virtual void UnBind() override;

private:
    Ptr<CTexture>   m_OutTex;
    CStructBuffer*   m_StructBufferTest;
    Vec4            m_vecSBuffer[32];

public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_OutTex = _Tex; }
    void SetColor(const Vec3& _RGB) { GetSharedCBuffer().arrV4[0] = _RGB; }
};

