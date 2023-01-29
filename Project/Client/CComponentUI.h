#pragma once
#include "CUI.h"

#include <Engine/CRes.h>
#include <Engine/ptr.h>

class CGameObject;

class CComponentUI :
    public CUI
{
private:
    CComponentUI() = delete;
public:
    CComponentUI(const string& _ID, eCOMPONENT_TYPE _Type);
    virtual ~CComponentUI();

private:
    CGameObject* m_pTarget;
    const eCOMPONENT_TYPE m_Type;

public:
    //Setter
    void SetTarget(CGameObject* _pTarget);

    //Getter
    CGameObject* GetTarget() { return m_pTarget; }
    eCOMPONENT_TYPE GetComponentType() { return m_Type; }

    //인자로 들어온 버퍼에 wstring 타입의 키값을 string 형태로 바꿔서 넣어주는 함수
    bool GetResKey(Ptr<CRes> _Res, __out string& _out);

public:
    virtual void tick() override;
    virtual int render_update() override;



};