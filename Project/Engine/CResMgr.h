#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CPathMgr.h"

//템플릿에 사용되는 리소스들
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CComputeShader.h"
#include "CAnim2DAtlas.h"

//새 Res 추가
//1. define.h enum에 Res 타입 추가했는지 확인
//2. m_umapResClassTypeIndex에 타입 인덱스와 eRES_TYPE을 바인딩

class CResMgr :
    public CSingleton<CResMgr>
{
    SINGLETON(CResMgr)
public:
    void init();
    void cleartick() { m_bResUpdated = false; }
    
private:
    unordered_map<string, Ptr<CRes>> m_arrRes[(UINT)eRES_TYPE::END];
    unordered_map<std::type_index, eRES_TYPE> m_umapResClassTypeIndex;

    //리소스 정보가 업데이트 되면 true로 변경
    bool    m_bResUpdated;



public:
    void CreateResClassTypeIndex();
    void CreateDefaultMesh();

    void CreateDefaultShader();
    bool CreateDefaultGraphicsShader();
    bool CreateDefaultComputeShader();

    void CreateDefaultMaterial();
    void LoadDefaultTexture();

    void CreateDefaultAnimAtlas();

public:
    template <typename T>
    eRES_TYPE GetResType();

    bool IsUpdated() const { return m_bResUpdated; }

    template<typename T>
    Ptr<T> FindRes(const string& _strKey);

    template<typename T>
    void AddRes(const string& _strKey, Ptr<T>& _Res);

    template<typename T>
    Ptr<T> Load(const string& _strKey, const std::filesystem::path& _path);

    const unordered_map<string, Ptr<CRes>>& GetResMap(eRES_TYPE _ResType);

    Ptr<CTexture> CreateTexture(const string& _strKey, UINT _uWidth, UINT _uHeight, DXGI_FORMAT _PixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage);
};

template<typename T>
inline eRES_TYPE CResMgr::GetResType()
{
    return m_umapResClassTypeIndex[std::type_index(typeid(T))];
}


template<typename T>
inline Ptr<T> CResMgr::FindRes(const string& _strKey)
{
    eRES_TYPE type = CResMgr::GetInst()->GetResType<T>();
      
    auto iter = m_arrRes[(UINT)type].find(_strKey);
    if (iter == m_arrRes[(UINT)type].end())
        return nullptr;

    //if (type == eRES_TYPE::MATERIAL)
    //    return (T*)iter->second.Get()->Clone();

    return iter->second;    
}


template<typename T>
inline void CResMgr::AddRes(const string& _strKey, Ptr<T>& _Res)
{
    // 중복키로 리소스 추가하려는 경우
    assert( ! FindRes<T>(_strKey).Get() );

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);
}


template<typename T>
inline Ptr<T> CResMgr::Load(const string& _strKey, const std::filesystem::path& _path)
{
    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();
    
    // 이미 해당 키로 리소스가 있다면, 반환
    if (nullptr != pRes)
        return (T*)pRes.Get();
        
    pRes = new T;
    pRes->SetKey(_strKey);
    pRes->SetRelativePath(_path);


    if (false == pRes->Load(_path))
        return nullptr;

    eRES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, pRes));

    m_bResUpdated = true;

    return (T*)pRes.Get();
}

inline const unordered_map<string, Ptr<CRes>>& CResMgr::GetResMap(eRES_TYPE _ResType)
{
    return m_arrRes[(UINT)_ResType];
}

inline Ptr<CTexture> CResMgr::CreateTexture(const string& _strKey, UINT _uWidth, UINT _uHeight, DXGI_FORMAT _PixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage)
{
    Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

    assert(nullptr == pTex);

    pTex = new CTexture;
    if (FAILED(pTex->Create(_uWidth, _uHeight, _PixelFormat, _D3D11_BIND_FLAG, _Usage)))
    {
        assert(nullptr);
    }

    AddRes<CTexture>(_strKey, pTex);

    return pTex;
}
