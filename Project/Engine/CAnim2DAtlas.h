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
    //�ִϸ��̼��� �����Ӻ� �ε���
    int uIdx;

    vector<std::function<void()>> pfuncCallback;
};

struct tAnimFrameIdx
{
    string strAnimName;
    vector<tAnimFrame> vecFrame;

    Vec2 vPivot;
    //vecFrame.size()�� �� ���� �ٸ� �� ����. ���� ������ ���� ���� �����ӿ� �ٸ� ������ ������� �� ��� ���
    UINT                uNumFrame;
    float               fFullPlayTime;
    float               fTimePerFrame;  //�� ��ġ / ������ ���� ������(�� �����Ӵ� �ð�)
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

    //�� �ִϸ��̼� �������� UV ����
    //�� �����ʹ� �ѹ� �����Ǹ� �����ϱ� �������� GPU�� �ö� ���� �״�� ��������
    vector<tAnimFrameUV> m_vecFrameUV;

    //���� �ִϸ��̼��� ������ ��ȣ�� ����Ǿ��ִ� ���͸� ����ִ� ����
    //���� �ִϸ��̼��� ����Ǵ� ���
    unordered_map<string, tAnimFrameIdx> m_mapAnim;


public:
    void SetAtlasTexture(Ptr<CTexture> _AtlasTex);
    void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);
    void SetNewAnimUV(UINT _uNumCol, UINT _uNumRow);

    void AddAnim2D(const string& _strAnimKey, const tAnimFrameIdx& _vecAnimFrameIdx);
    const tAnimFrameIdx* FindAnim2D(const string& _AnimIdxStrKey);

    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    const tAnimFrameUV& GetFrameUVData(int _iIdx) { return m_vecFrameUV[_iIdx]; }
};