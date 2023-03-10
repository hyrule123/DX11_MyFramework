#pragma once
#include "CEntity.h"

#include "global.h"

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
    const eRES_TYPE  m_Type;
    int             m_iRefCount;

    string         m_strKey;
    wstring         m_strRelativePath;

public:
    const string& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

    eRES_TYPE GetResType() const { return m_Type; }

private:
    void SetKey(const string& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

    void AddRef() { ++m_iRefCount; }
    void Release();

    // 리소스 바인딩
    virtual void BindData() = 0;

private:
    // 파일로부터 로딩
    virtual int Load(const wstring& _strFilePath) = 0;

public:
    // 파일로 저장
    virtual int Save(const wstring&) = 0;

    // 리소스는 Clone 을 구현하지 않는다.(Material 제외)
    virtual CRes* Clone() { return nullptr; assert(nullptr); }



};

