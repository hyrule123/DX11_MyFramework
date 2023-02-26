#pragma once
#include "CRes.h"

#include "ptr.h"

enum class eANIM_TYPE
{
    SEQUENTIAL,         //���� ���
    DIRECTIONAL_COL_HALF_FLIP   //0��~180�� ���� �ִϸ��̼Ǹ� ����. �Ѿ�� �ִϸ��̼��� FLIP�� ����� ��.
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
    //�ִϸ��̼��� �����Ӻ� �ε���
    UINT uIdxInVecFrameUV;

    vector<std::function<void()>> pfuncCallback;
};

struct tAnimFrameIdx
{
    string strAnimName;
    vector<tAnimFrame> vecFrame;

    
    //vecFrame.size()�� �� ���� �ٸ� �� ����. ���� ������ ���� ���� �����ӿ� �̹����� ������� �� ��� ���
    UINT                uNumFrame;
    float               fFullPlayTime;

    //���� ��ü ����ð� / ������ �� �Ѱ�(�� �����Ӵ� �ð�) 
    //�ڵ� ���
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
    virtual int Load(const wstring& _strFilePath) override { return 0; };
    virtual int Save(const wstring&) override { return 0; };

private:
    //�ִϸ��̼��� ����� �Ǵ� �ؽ�ó
    Ptr<CTexture>       m_AtlasTex; 

    //�������� UV ���� ��ü ����
    vector<tAnimFrameUV> m_vecFrameUV;

    //���� �ִϸ��̼��� ������ ��ȣ�� ����Ǿ��ִ� ���͸� ����ִ� ����
    //���� �ִϸ��̼��� ����Ǵ� ���
    unordered_map<string, tAnimFrameIdx> m_mapAnim;

    //�׸��� ������ �ִϸ��̼��� ��� ���
    UINT m_uRowTotal;
    UINT m_uColTotal;

public:
    void SetAtlasTexture(Ptr<CTexture> _AtlasTex);

    void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal);
    void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);
    void SetNewAnimUV_SC_Redundant(UINT _uRowTotal, UINT _uRowStart, UINT _uRowPitch);


    //�ִϸ��̼� ���� �޼ҵ�
    //================================================================================================================
    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, const tAnimFrameIdx& _vecAnimFrameIdx, 
         float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f));

    //�ִϸ��̼��� ���鶧�� ��ü ���� ������ ����. �������� �ȿ��� �����
    tAnimFrameIdx* AddAnim2D(const string& _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );
    
    tAnimFrameIdx* AddAnim2D_SC_Redundant(
        const string& _strAnimKey, UINT _uRowStart, UINT _uRowPitch,
        float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f)
    );

    //�� ��������Ʈ�� ���ӵ� ������ �ƴ� ������ ������ ��� ���
    tAnimFrameIdx* AddAnim2D_vecRowIndex(const string& _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f));
    //=================================================================================================================


    const tAnimFrameIdx* FindAnim2D(const string& _AnimIdxStrKey);

    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    const tAnimFrameUV& GetFrameUVData(int _iIdx) { return m_vecFrameUV[_iIdx]; }
};