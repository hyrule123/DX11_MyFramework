#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CPathMgr.h"

//���ø��� ���Ǵ� ���ҽ���
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"
#include "CPrefab.h"

//�� Res �߰�
//1. define.h enum�� Res Ÿ�� �߰��ߴ��� Ȯ��
//2. m_umapResClassTypeIndex�� Ÿ�� �ε����� eRES_TYPE�� ���ε�


class CResMgr :
    public CSingleton<CResMgr>
{
    SINGLETON(CResMgr)
private:
    unordered_map<wstring, Ptr<CRes>> m_arrRes[(UINT)eRES_TYPE::END];
    unordered_map<std::type_index, eRES_TYPE> m_umapResClassTypeIndex;

public:
    void init();

private:
    void CreateResClassTypeIndex();
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultMaterial();
    void LoadDefaultTexture();

public:
    template <typename T>
    eRES_TYPE GetResType();

    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);

    template<typename T>
    void AddRes(const wstring& _strKey, Ptr<T>& _Res);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);
};

template<typename T>
inline eRES_TYPE CResMgr::GetResType()
{
    return m_umapResClassTypeIndex[std::type_index(typeid(T))];


    //const std::type_index& mesh = std::type_index(typeid(CMesh));
    ////const std::type_index& meshdata = std::type_index(typeid(CMeshData));
    //const std::type_index& material = std::type_index(typeid(CMaterial));
    //const std::type_index& texture = std::type_index(typeid(CTexture));
    ////const std::type_index& sound = std::type_index(typeid(CSound));
    ////const std::type_index& prefab = std::type_index(typeid(CPrefab));
    //const std::type_index& gs = std::type_index(typeid(CGraphicsShader));
    ////const std::type_index& cs = std::type_index(typeid(CComputeShader));

    //if (typeid(T).hash_code() == mesh.hash_code())
    //    return eRES_TYPE::MESH;
    //if (typeid(T).hash_code() == gs.hash_code())
    //    return eRES_TYPE::GRAPHICS_SHADER;
    //if (typeid(T).hash_code() == texture.hash_code())
    //    return eRES_TYPE::TEXTURE;
    //if (typeid(T).hash_code() == material.hash_code())
    //    return eRES_TYPE::MATERIAL;

    //return eRES_TYPE::END;
}


template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    eRES_TYPE type = CResMgr::GetInst()->GetResType<T>();
      
    auto iter = m_arrRes[(UINT)type].find(_strKey);
    if (iter == m_arrRes[(UINT)type].end())
        return nullptr;

    if (type == eRES_TYPE::MATERIAL)
        return (T*)iter->second.Get()->Clone();

    return (T*)iter->second.Get();    
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T>& _Res)
{
    // �ߺ�Ű�� ���ҽ� �߰��Ϸ��� ���
    assert( ! FindRes<T>(_strKey).Get() );

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);
}


template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();
    
    // �̹� �ش� Ű�� ���ҽ��� �ִٸ�, ��ȯ
    if (nullptr != pRes)
        return (T*)pRes.Get();
        
    pRes = new T;
    pRes->SetKey(_strKey);
    pRes->SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    if (FAILED(pRes->Load(strFilePath)))
    {
        assert(nullptr);
    }

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, pRes));

    return (T*)pRes.Get();
}
