#pragma once
#include "CUI_BasicWindow.h"

class CAnim2DAtlas;
class CUI_DragNDropReceiver;

class CUIobj_AnimEditor :
    public CUI_BasicWindow
{
public:
    CUIobj_AnimEditor();
    virtual ~CUIobj_AnimEditor();

public:
    virtual void init() override;
    virtual void tick() override;

    virtual void render_update() override;

private:
    CUI_DragNDropReceiver* m_AnimAtlasReceiver;
    Ptr<CAnim2DAtlas> m_CurrentTarget;

private:


};



////1. 아틀라스 텍스처 등록
//void SetAtlasTexture(Ptr<CTexture> _AtlasTex);
//
////2. 프레임별 UV 값 등록
//
////수동으로 계산해서 등록
//void SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV);
//
////규칙적인 사이즈일 경우 등분해서 등록
//void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal);
//void SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch);
//
////중복된 방향(180도 8방향)을 가지는 애니메이션 프레임을 만들 때
//void SetNewAnimUV_SC_Redundant(UINT _uRowTotal, UINT _uRowStart, UINT _uRowPitch);


////애니메이션 생성 메소드
////================================================================================================================
//tAnim2D* AddAnim2D(const string& _strAnimKey, const tAnim2D& _vecAnimFrameIdx,
//    float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
//);
//
//tAnim2D* AddAnim2D(const string& _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f));
//
////애니메이션을 만들때는 전체 열의 갯수만 받음. 나머지는 안에서 계산함
//tAnim2D* AddAnim2D(const string& _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
//    float _fFullPlayTime, eANIM_TYPE _eAnimType = eANIM_TYPE::SEQUENTIAL, Vec2 _vPivot = Vec2(0.5f, 0.5f)
//);
//
//tAnim2D* AddAnim2D_SC_Redundant(
//    const string& _strAnimKey, UINT _uRowStart, UINT _uRowPitch,
//    float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f)
//);
//
////각 스프라이트가 연속된 정수가 아닌 떨어진 숫자일 경우 사용
//tAnim2D* AddAnim2D_vecRowIndex(const string& _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot = Vec2(0.5f, 0.5f));
