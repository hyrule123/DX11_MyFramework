#pragma once
#include "cEntity.h"
#include "cComputeShader.h"

#include <UtilLib_DLL/json/forwards.h>

//Script 프로젝트에서 직접 커스터마이징 가능한 쉐이더 데이터(상수버퍼 + 구조화버퍼) 클래스

class cComputeShader;
class cShderModule :
    public cEntity
{
public:
    cShderModule();
    virtual ~cShderModule();

    cShderModule(const cShderModule& _other) = delete;
    CLONE_DISABLE(cShderModule);

    //저장할일이 있을지는 모르겠지만 일단 사용
    virtual bool SaveJson(Json::Value* _pJval) { return true; }
    virtual bool LoadJson(const Json::Value* _pJval) { return true; }

    virtual bool Init() { return true; };

    //두 함수를 각자 사용 용도에 맞게 재정의할 것
    virtual bool BindDataUAV() = 0;
    virtual bool BIndDataSRV() = 0;
    virtual void UnBind() = 0;

private:
    cComputeShader* m_pOwner;
public:
    void SetOwner(cComputeShader* _pShader) { assert(_pShader); m_pOwner = _pShader; }
    cComputeShader* GetOwner() const { return m_pOwner; }
};

