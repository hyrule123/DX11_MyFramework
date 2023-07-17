#pragma once
#include "cShaderDataModule.h"
class cShaderData_Default :
    public cShaderDataModule
{
    cShaderData_Default();
    virtual ~cShaderData_Default();

    cShaderData_Default(const cShaderData_Default& _other) = delete;
    CLONE_DISABLE(cShaderData_Default);

    //두 함수를 각자 사용 용도에 맞게 재정의
    //컴퓨트 쉐이더 사용시에는 데이터의 갯수를 같이 반환한다.
    virtual tNumDataCS BindDataCS() override;
    virtual bool BindDataGS() override;
    virtual void UnBind() override;

private:
    tMtrlScalarData          m_CBuffer_CSShared;

public:
    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);
};

