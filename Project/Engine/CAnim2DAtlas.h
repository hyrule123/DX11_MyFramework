#pragma once
#include "CRes.h"

#include "ptr.h"

#include <UtilLib_DLL/json/forwards.h>

enum class eANIM_TYPE
{
    SEQUENTIAL,         //순차 재생
    DIRECTIONAL_COL_HALF_FLIP   //0도~180도 기준 애니메이션만 존재. 넘어가는 애니메이션은 FLIP을 해줘야 함.
};

struct tAnimFrameUV
{
    Vec2 v2_UVLeftTop;
    Vec2 v2_UVSlice;

    Vec2 v2_Offset;
    Vec2 Padding;
};

//struct tAnimFrame
//{
//    //애니메이션의 프레임별 인덱스
//    UINT uIdxInVecFrameUV;
//
//    vector<std::function<void()>> pfuncCallback;
//};

//실제 애니메이션 정보가 저장되어있는 구조체.
struct tAnim2D
{
    string strKeyAnim2D;
    vector<UINT> vecFrame;

    //1차원: 프레임 번호, 2차원: 벡터 내부 콜백 함수
    vector<vector<std::function<void()>>> vec2D_pFuncCallback;

    
    //vecFrame.size()와 이 값은 다를 수 있음. 방향 정보에 따라 같은 프레임에 이미지를 보여줘야 할 경우 등등
    UINT                uNumFrame;
    float               fFullPlayTime;

    //위의 전체 재생시간 / 프레임 수 한것(한 프레임당 시간) 
    //자동 계산
    float               fTimePerFrame;  

    eANIM_TYPE          eAnimType;
    Vec2 vPivot;

    UINT                uColTotal;
    UINT                uRowTotal;

    tAnim2D() : vec2D_pFuncCallback(0), uNumFrame(), fFullPlayTime(), fTimePerFrame(), eAnimType(), uColTotal(), uRowTotal()
    {}
};

class CTexture;
class CAnim2DAtlas :
    public CRes
{
public:
    CAnim2DAtlas();
    virtual ~CAnim2DAtlas();
public:
    //json 파일에 숫자 데이터만 들어가 있기에 가독성이 중요하지 않아서 붙여쓰기 방식으로 작성하기 위해 오버라이드 하였음.
    //virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool SaveJson(Json::Value* _jVal) override;
    virtual bool LoadJson(Json::Value* _jVal) override;

    virtual void BindData() override {};
    

private:
    //애니메이션의 대상이 되는 텍스처
    Ptr<CTexture>       m_AtlasTex; 

    //프레임의 UV 정보 전체 모음
    vector<tAnimFrameUV> m_vecFrameUV;

    //개별 애니메이션의 프레임 번호가 저장되어있는 벡터를 들고있는 변수
    //실제 애니메이션이 저장되는 장소
    unordered_map<string, tAnim2D> m_mapAnim;

    //그리드 형태의 애니메이션일 경우 사용
    UINT m_uRowTotal;
    UINT m_uColTotal;

    //모든 프레임이 같은 사이즈를 가지고있는지 여부 및 그 사이즈
    bool m_bRegularFrameSize;
    Vec2 m_v2FrameSize;

public:
    //1. 아틀라스 텍스처 등록
    void SetAtlasTexture(Ptr<CTexture> _AtlasTex);
    Ptr<CTexture> GetAtlasTex() const { return m_AtlasTex; }

    //2. 프레임별 UV 값 등록

    //수동으로 계산해서 등록
    void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);

    //규칙적인 사이즈일 경우 등분해서 등록
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);

    //중복된 방향(180도 8방향)을 가지는 애니메이션 프레임을 만들 때
    void SetNewAnimUV_SC_Redundant(UINT _uRowTotal, UINT _uRowStart, UINT _uRowPitch);


    //애니메이션 생성 메소드
    //================================================================================================================
    tAnim2D* AddAnim2D(const string& _strAnimKey, const tAnim2D& _vecAnimFrameIdx, 
         float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    tAnim2D* AddAnim2D(const string& _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f));

    //애니메이션을 만들때는 전체 열의 갯수만 받음. 나머지는 안에서 계산함
    tAnim2D* AddAnim2D(const string& _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );
    
    tAnim2D* AddAnim2D_SC_Redundant(
        const string& _strAnimKey, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    //각 스프라이트가 연속된 정수가 아닌 떨어진 숫자일 경우 사용
    tAnim2D* AddAnim2D_vecRowIndex(const string& _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f));
    //=================================================================================================================


    const tAnim2D* FindAnim2D(const string& _AnimIdxStrKey);
    
    const tAnimFrameUV& GetFrameUVData(int _iIdx) { return m_vecFrameUV[_iIdx]; }

    bool IsFrameSizeRegular() const { return m_bRegularFrameSize; }
    Vec2 GetFrameSize(UINT _uIdxFrameUV) const;
};
