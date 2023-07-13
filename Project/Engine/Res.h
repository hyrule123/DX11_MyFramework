#pragma once
#include "Entity.h"

#include "global.h"
#include "strKey_Default.h"


#include <UtilLib_DLL/json/forwards.h>

//각자 리소스 타입에 맞는 경로를 받아옴.
#define GETRESPATH PathMgr::GetInst()->GetPathRel_Resource(GetResType())

namespace JsonKey_Res
{
    STRKEY_DECLARE(m_eResType);
}

class Res :
    public Entity
{
public:
    Res(eRES_TYPE _type);

    Res(const Res& _other);
    virtual ~Res();

    friend class ResMgr;

    template<typename T>
    friend class Ptr;

private:
    int             m_iRefCount;
    eRES_TYPE       m_eResType;

    bool            m_bEngineDefaultRes;

public:
    eRES_TYPE GetResType() const { return m_eResType; }

    int GetRefCount() const { return m_iRefCount; }

    void SetEngineDefaultRes(bool _bDefault) { m_bEngineDefaultRes = _bDefault; }
    bool IsEngineDefaultRes() const { return m_bEngineDefaultRes; }

private:
    void AddRef() { ++m_iRefCount; }
    void Release() { if (0 == --m_iRefCount) delete this; };

    // 리소스 바인딩
    virtual void BindData() = 0;

public:
    // 파일로부터 로딩
    virtual bool Load(const std::filesystem::path& _fileName);
    virtual bool LoadJson(Json::Value* _pJson);

public:
    // 파일로 저장
    virtual bool Save(const std::filesystem::path& _fileName);
    virtual bool SaveJson(Json::Value* _pJson);

    // 리소스는 Clone 을 구현하지 않는다.(Material 제외)
    virtual Res* Clone() { return nullptr; assert(nullptr); }
};

