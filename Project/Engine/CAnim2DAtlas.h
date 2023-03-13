#pragma once
#include "CRes.h"

#include "ptr.h"

enum class eANIM_TYPE
{
    SEQUENTIAL,         //순차 재생
    DIRECTIONAL_COL_HALF_FLIP   //0도~180도 기준 애니메이션만 존재. 넘어가는 애니메이션은 FLIP을 해줘야 함.
};

struct tAnimFrameUV
{
    Vec2 LeftTopUV;
    Vec2 SliceUV;

    Vec2 Offset;
    Vec2 Padding;
};

struct tAnimFrame
{
    //애니메이션의 프레임별 인덱스
    UINT uIdxInVecFrameUV;

    vector<std::function<void()>> pfuncCallback;
};

struct tAnimFrameIdx
{
    string strAnimName;
    vector<tAnimFrame> vecFrame;

    
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
};

class CTexture;
class CAnim2DAtlas :
    public CRes
{
public:
    CAnim2DAtlas();
    virtual ~CAnim2DAtlas();
public:
    virtual void BindData() override {};
    virtual bool Load(const std::filesystem::path& _path) override { return false; };
    virtual bool Save(const std::filesystem::path& _path) override { return false; };

private:
    //애니메이션의 대상이 되는 텍스처
    Ptr<CTexture>       m_AtlasTex; 

    //프레임의 UV 정보 전체 모음
    vector<tAnimFrameUV> m_vecFrameUV;

    //개별 애니메이션의 프레임 번호가 저장되어있는 벡터를 들고있는 변수
    //실제 애니메이션이 저장되는 장소
    unordered_map<string, tAnimFrameIdx> m_mapAnim;

    //그리드 형태의 애니메이션일 경우 사용
    UINT m_uRowTotal;
    UINT m_uColTotal;

public:
    void SetAtlasTexture(Ptr<CTexture> _AtlasTex);

    void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);
    void SetNewAnimUV_SC_Redundant(UINT _uRowTotal, UINT _uRowStart, UINT _uRowPitch);


    //애니메이션 생성 메소드
    //================================================================================================================
    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, const tAnimFrameIdx& _vecAnimFrameIdx, 
         float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f));

    //애니메이션을 만들때는 전체 열의 갯수만 받음. 나머지는 안에서 계산함
    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );
    
    tAnimFrameIdx* AddAnim2D_SC_Redundant(
        const string& _strAnimKey, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    //각 스프라이트가 연속된 정수가 아닌 떨어진 숫자일 경우 사용
    tAnimFrameIdx* AddAnim2D_vecRowIndex(const string& _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f));
    //=================================================================================================================


    const tAnimFrameIdx* FindAnim2D(const string& _AnimIdxStrKey);

    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    const tAnimFrameUV& GetFrameUVData(int _iIdx) { return m_vecFrameUV[_iIdx]; }
};
