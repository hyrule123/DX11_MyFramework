#pragma once
#include "CRes.h"

#include "ptr.h"

enum class eANIM_TEX_FRAME_TYPE
{
    UNORDERED,
    DIRECTIONAL_ROW,
    DIRECTIONAL_COL
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
    UINT uIdx;

    vector<std::function<void()>> pfuncCallback;
};

struct tAnimFrameIdx
{
    string strAnimName;
    vector<tAnimFrame> vecFrame;

    Vec2 vPivot;
    //vecFrame.size()와 이 값은 다를 수 있음. 방향 정보에 따라 같은 프레임에 다른 정보를 보여줘야 할 경우 등등
    UINT                uNumFrame;
    float               fFullPlayTime;
    float               fTimePerFrame;  //위 수치 / 프레임 수로 나눈것(한 프레임당 시간)
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
    virtual int Load(const wstring& _strFilePath) override { return 0; };
    virtual int Save(const wstring&) override { return 0; };

private:
    //애니메이션의 대상이 되는 텍스처
    Ptr<CTexture>       m_AtlasTex; 

    //프레임의 UV 정보 전체 모음
    vector<tAnimFrameUV> m_vecFrameUV;

    //개별 애니메이션의 프레임 번호가 저장되어있는 벡터를 들고있는 변수
    //실제 애니메이션이 저장되는 장소
    unordered_map<string, tAnimFrameIdx> m_mapAnim;


public:
    void SetAtlasTexture(Ptr<CTexture> _AtlasTex);
    void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);

    void AddAnim2D(const string& _strAnimKey, const tAnimFrameIdx& _vecAnimFrameIdx);

    //애니메이션을 만들때는 전체 열의 갯수만 받음. 나머지는 안에서 계산함
    void AddAnim2D(const string& _strAnimKey, UINT _uColTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);

    const tAnimFrameIdx* FindAnim2D(const string& _AnimIdxStrKey);

    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    const tAnimFrameUV& GetFrameUVData(int _iIdx) { return m_vecFrameUV[_iIdx]; }
};