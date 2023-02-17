#pragma once
#include "CEntity.h"

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

    // ���ҽ� ���ε�
    virtual void BindData() = 0;

private:
    // ���Ϸκ��� �ε�
    virtual int Load(const wstring& _strFilePath) = 0;

public:
    // ���Ϸ� ����
    virtual int Save(const wstring&) = 0;

    // ���ҽ��� Clone �� �������� �ʴ´�.(Material ����)
    virtual CRes* Clone() { return nullptr; assert(nullptr); }



};

