#pragma once
#include "CEntity.h"

#include "global.h"

#include <UtilLib_DLL/json/forwards.h>

class CRes :
    public CEntity
{
public:
    CRes(eRES_TYPE _type);
    CRes(const CRes& _other);
    virtual ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;

private:
    int             m_iRefCount;

    eRES_TYPE  m_eResType;
    string         m_strKey;
    //std::filesystem::path         m_RelativePath;

    

public:
    void SetKey(const string& _strKey) { m_strKey = _strKey; }
    const string& GetKey() { return m_strKey; }

    eRES_TYPE GetResType() const { return m_eResType; }

private:
    
    

    void AddRef() { ++m_iRefCount; }
    void Release();

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
    virtual CRes* Clone() { return nullptr; assert(nullptr); }



};

