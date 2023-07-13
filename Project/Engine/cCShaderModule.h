#pragma once
#include "cEntity.h"
#include "cComputeShader.h"

#include <UtilLib_DLL/json/forwards.h>

class cComputeShader;
class cCShaderModule :
    public cEntity
{
public:
    cCShaderModule();
    virtual ~cCShaderModule();

    cCShaderModule(const cCShaderModule& _other) = delete;
    CLONE_DISABLE(cCShaderModule);

    virtual bool SaveJson(Json::Value* _pJval) { return true; }
    virtual bool LoadJson(const Json::Value* _pJval) { return true; }

    virtual bool Init() { return true; };
    virtual bool BindData() = 0;
    virtual void UnBind() = 0;

private:
    cComputeShader* m_pOwner;
public:
    void SetOwner(cComputeShader* _pShader) { assert(_pShader); m_pOwner = _pShader; }
    cComputeShader* GetOwner() const { return m_pOwner; }
};

