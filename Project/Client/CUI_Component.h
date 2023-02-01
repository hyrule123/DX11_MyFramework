#pragma once
#include "CUI.h"

#include <Engine/CRes.h>
#include <Engine/ptr.h>

class CGameObject;

class CUI_Component :
    public CUI
{
private:
    CUI_Component() = delete;
public:
    CUI_Component(const string& _ID, eCOMPONENT_TYPE _Type);
    virtual ~CUI_Component();

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
    const string& GetResKey(Ptr<CRes> _Res);

public:
    virtual void tick() override;
    virtual int render_update() override;



};