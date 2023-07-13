#pragma once
#include "Singleton.h"

#include "struct.h"

#include "Ptr.h"
#include "cPathMgr.h"

//템플릿에 사용되는 리소스들
#include "cMesh.h"
#include "cTexture.h"
#include "CGraphicsShader.h"
#include "cMaterial.h"
#include "CPrefab.h"
#include "cComputeShader.h"
#include "cAnim2DAtlas.h"

#include <type_traits>

//새 cRes 추가
//1. define.h enum에 cRes 타입 추가했는지 확인
//2. m_umapResClassTypeIndex에 타입 인덱스와 eRES_TYPE을 바인딩

class cResMgr
    : public Singleton<cResMgr>
{
    SINGLETON(cResMgr);

public:
    void init();
    void cleartick() { m_bResUpdated = false; }
    
private:
    unordered_map<string, Ptr<cRes>, tHasher_String, std::equal_to<>> m_arrRes[(UINT)eRES_TYPE::END];

    //리소스 정보가 업데이트 되면 true로 변경
    bool    m_bResUpdated;

private:

    void CreateDefaultMesh();
    void CreateDefaultShader();

    bool CreateDefaultGraphicsShader();
    bool CreateDefaultComputeShader();

    void CreateDefaultMaterial();
    void LoadDefaultTexture();

    bool LoadUserGraphicsShaderAll();

public:
    bool IsUpdated() const { return m_bResUpdated; }

    template <typename T>
    eRES_TYPE GetResType();

    template<typename T>
    Ptr<T> FindRes(const string_view _strKey);

    //map에서 데이터를 제거한다. 
    //레퍼런스 카운트가 0이 되지 않으면 실제로 데이터가 제거되지 않으므로 주의할 것.
    template<typename T>
    void DeleteRes(const string_view _strKey);

    template<typename T>
    void AddRes(const string_view _strKey, Ptr<T>& _Res);
private:
    //SFINAE(Substitution Failure Is Not An Error)를 통한 리소스 초기화 함수 오버로딩 연습
    //취소 - 여러 개의 타입에 대해 오버로딩이 모호함.
    // 
    //함수 설명:
    //위 함수는 T가 cComputeShader일 경우에 호출되고
    //아래 함수는 T가 cComputeShader가 아닐 경우에 호출됨.
    //같은 표현인데 느낌표만 붙인거임
    //SFINAE를 이용해서 한 쪽으로 캐스팅이 실패하면 다른 오버로딩 함수가 호출되도록 결정 됨.
    //template<typename T>
    //typename std::enable_if<std::is_base_of<cComputeShader, T>::value>::type
    //     initRes(Ptr<T>& _Res);

    //template <typename T, typename = std::enable_if_t<!std::is_base_of<cComputeShader, T>::value>>
    //   void initRes(Ptr<T>& _Res);

    
public:
    //Ptr<cRes> Load(eRES_TYPE _eResType, const std::filesystem::path& _fileName);



    //파일명 = 키일떄 사용

    //파일명 규칙 : 특정 리소스 폴더 아래부터의 주소
    //ex) Content/cTexture/Marine/Marine.bmp -> Marine/Marine.bmp
    template<typename T>
    Ptr<T> Load(const std::filesystem::path& _fileName, const string_view _strKey = "");

    unordered_map<string, Ptr<cRes>, tHasher_String, std::equal_to<>> const& GetResMap(eRES_TYPE _ResType);

    Ptr<cTexture> CreateTexture(const string_view _strKey, UINT _uWidth, UINT _uHeight, DXGI_FORMAT _PixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage);
};

template<typename T>
inline eRES_TYPE cResMgr::GetResType()
{
    static_assert(std::is_base_of_v<cRes, T>);


    if constexpr (std::is_base_of_v<cMesh, T>)
    {
        return eRES_TYPE::MESH;
    }

    //if constexpr (std::is_base_of_v<CMeshData, T>)
    //{
    //    return eRES_TYPE::MESHDATA;
    //}

    else if constexpr (std::is_base_of_v<cMaterial, T>)
    {
        return eRES_TYPE::MATERIAL;
    }

    else if constexpr (std::is_base_of_v<cTexture, T>)
    {
        return eRES_TYPE::TEXTURE;
    }

    else if constexpr (std::is_base_of_v<cAnim2DAtlas, T>)
    {
        return eRES_TYPE::ANIM2D_ATLAS;
    }

    //else if constexpr (std::is_base_of_v<CSound, T>)
    //{
    //    return eRES_TYPE::SOUND;
    //}

    else if constexpr (std::is_base_of_v<CPrefab, T>)
    {
        return eRES_TYPE::PREFAB;
    }

    else if constexpr (std::is_base_of_v<CGraphicsShader, T>)
    {
        return eRES_TYPE::GRAPHICS_SHADER;
    }

    else if constexpr (std::is_base_of_v<cComputeShader, T>)
    {
        return eRES_TYPE::COMPUTE_SHADER;
    }

    else
        return eRES_TYPE::UNKNOWN;
}


template<typename T>
inline Ptr<T> cResMgr::FindRes(const string_view _strKey)
{
    eRES_TYPE resType = GetResType<T>();

    if (eRES_TYPE::UNKNOWN == resType)
        return nullptr;
      
    auto iter = m_arrRes[(UINT)resType].find(_strKey);
    if (iter == m_arrRes[(UINT)resType].end())
        return nullptr;

    return iter->second;    
}

template<typename T>
inline void cResMgr::DeleteRes(const string_view _strKey)
{
    eRES_TYPE Type = GetResType<T>();

    if (eRES_TYPE::UNKNOWN == Type)
        return;

    const auto& iter = m_arrRes[(int)Type].find(_strKey);
    if (m_arrRes[(int)Type].end() != iter)
    {
        m_arrRes[(int)Type].erase(iter);
    }
}


template<typename T>
inline void cResMgr::AddRes(const string_view _strKey, Ptr<T>& _Res)
{
    static_assert(std::is_base_of<cRes, T>::value, "Type T is not Derived class of cRes!!");

    // 중복키로 리소스 추가하려는 경우
    assert( ! FindRes<T>(_strKey).Get() );

    //키값을 설정(Load를 거치지 않고 AddRes 되는 경우도 있으므로 여기서 해줘야 함)
    _Res->SetKey(_strKey);

    eRES_TYPE type = GetResType<T>();

    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
}


template<typename T>
inline Ptr<T> cResMgr::Load(const std::filesystem::path& _fileName, const string_view _strKey)
{
    //cRes를 상속받는 클래스가 아닐 경우 컴파일 중지
    static_assert(std::is_base_of<cRes, T>::value);

    string strKey(_strKey);

    //Key가 비어있을 경우 파일명을 키값으로 사용한다.
    if (strKey.empty())
        strKey = _fileName.string();

    Ptr<T> pRes = FindRes<T>(strKey).Get();
    
    // 이미 해당 키로 리소스가 있다면, 반환
    if (nullptr != pRes)
        return pRes;
        
    pRes = new T;

    if (false == pRes->Load(_fileName))
        return nullptr;

    AddRes<T>(strKey, pRes);

    m_bResUpdated = true;

    return pRes;
}


inline unordered_map<string, Ptr<cRes>, tHasher_String, std::equal_to<>> const& cResMgr::GetResMap(eRES_TYPE _ResType)
{
    return m_arrRes[(UINT)_ResType];
}

inline Ptr<cTexture> cResMgr::CreateTexture(const string_view _strKey, UINT _uWidth, UINT _uHeight, DXGI_FORMAT _PixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage)
{
    Ptr<cTexture> pTex = FindRes<cTexture>(_strKey);

    assert(nullptr == pTex);

    pTex = new cTexture;
    if (FAILED(pTex->Create(_uWidth, _uHeight, _PixelFormat, _D3D11_BIND_FLAG, _Usage)))
    {
        assert(nullptr);
    }

    AddRes<cTexture>(_strKey, pTex);

    return pTex;
}
