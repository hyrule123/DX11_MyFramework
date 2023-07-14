#pragma once
#include "cEntity.h"
#include "cComputeShader.h"

#include <UtilLib_DLL/json/forwards.h>

struct tNumData
{
    UINT X;
    UINT Y;
    UINT Z;
};

//Script 프로젝트에서 직접 커스터마이징 가능한 쉐이더 데이터(상수버퍼 + 구조화버퍼) 클래스

class cComputeShader;
class cShaderModule :
    public cEntity
{
public:
    cShaderModule();
    virtual ~cShaderModule();

    cShaderModule(const cShaderModule& _other) = delete;
    CLONE_DISABLE(cShaderModule);

    //저장할일이 있을지는 모르겠지만 일단 사용
    virtual bool SaveJson(Json::Value* _pJval) { return true; }
    virtual bool LoadJson(const Json::Value* _pJval) { return true; }

    virtual bool Init() { return true; };

    //두 함수를 각자 사용 용도에 맞게 재정의
    //컴퓨트 쉐이더 사용시에는 데이터의 갯수를 같이 반환한다.
    virtual tNumData BindDataCS() = 0;
    virtual bool BindDataGS() = 0;
    virtual void UnBind() = 0;
};

