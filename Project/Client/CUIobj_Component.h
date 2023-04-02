#pragma once
#include "CUI_BasicWindow.h"

#include <Engine/CRes.h>
#include <Engine/ptr.h>

class CGameObject;
class CComponent;

class CUIobj_Component :
    public CUI_BasicWindow
{
private:
    CUIobj_Component() = delete;
public:
    CUIobj_Component(const string& _ID, eCOMPONENT_TYPE _Type);
    virtual ~CUIobj_Component();

public:

    virtual bool beginUI() override;

    //재정의할 경우 반드시 이 함수를 호출할것
    virtual void render_update() override;

    virtual void endUI() override;


protected:
    //자신이 담당하는 컴포넌트가 없을 경우 호출되는 함수
    virtual void CreateNewComUI() {};

    //자신이 담당하는 컴포넌트가 있을 경우 호출되는 함수
    virtual void EditComUI() {};

    //CF)이외에 무조건 표시해야하는 UI는 render_update를 재정의해서 출력하면 됨.
    

private:
    CGameObject* m_pTarget;
    const eCOMPONENT_TYPE m_Type;

public:
    //Setter
    void SetTarget(CGameObject* _pTarget);

    //Getter
    CGameObject* GetTargetObj() { return m_pTarget; }
    eCOMPONENT_TYPE GetComponentType() { return m_Type; }
    CComponent* GetMyTargetComponent();

    //인자로 들어온 버퍼에 wstring 타입의 키값을 string 형태로 바꿔서 넣어주는 함수
    const string& GetResKey(Ptr<CRes> _Res);


};
