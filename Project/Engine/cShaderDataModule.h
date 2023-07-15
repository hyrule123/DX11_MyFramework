#pragma once
#include "IEntity.h"
#include "cComputeShader.h"

#include <UtilLib_DLL/json/forwards.h>

//Script 프로젝트에서 직접 커스터마이징 가능한 쉐이더 데이터(상수버퍼 + 구조화버퍼) 클래스

class cComputeShader;
class cShaderDataModule :
    public IEntity
{
public:
    cShaderDataModule();
    virtual ~cShaderDataModule();

    cShaderDataModule(const cShaderDataModule& _other) = delete;
    CLONE_DISABLE(cShaderDataModule);

    virtual bool Init() { return true; };

    //두 함수를 각자 사용 용도에 맞게 재정의
    //컴퓨트 쉐이더 사용시에는 데이터의 갯수를 같이 반환한다.
    virtual tNumData BindDataCS() = 0;
    virtual bool BindDataGS() = 0;
    virtual void UnBind() = 0;
};

