#pragma once
#include "CComponent.h"

#include "ptr.h"

class CAnim2D;
class CTexture;

class CAnimator2D :
    public CComponent
{
public:
    CAnimator2D();
    ~CAnimator2D();
    CLONE(CAnimator2D);

public:
    virtual void finaltick() override;

    virtual void cleanup() override {};

    void UpdateData();
    

private:
    map<string, CAnim2D*>  m_mapAnim;  // Animation ���
    CAnim2D* m_pCurAnim; // ���� ������� Animation
    bool                    m_bRepeat;  // �ݺ�

public:
    void Play(const string& _strName, bool _bRepeat);
    CAnim2D* FindAnim(const string& _strName);

    void CreateAnimation(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);

};

