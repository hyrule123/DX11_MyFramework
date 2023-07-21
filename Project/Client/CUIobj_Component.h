#pragma once
#include "cUI_BasicWindow.h"

#include <Engine/IRes.h>
#include <Engine/ptr.h>

class cGameObject;
class IComponent;

class cUIobj_Component :
    public cUI_BasicWindow
{
private:
    cUIobj_Component() = delete;
public:
    cUIobj_Component(const string& _ID, eCOMPONENT_TYPE _Type);
    virtual ~cUIobj_Component();

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
    cGameObject* m_pTarget;
    const eCOMPONENT_TYPE m_Type;

public:
    //Setter
    void SetTarget(cGameObject* _pTarget);

    //Getter
    cGameObject* GetTargetObj() { return m_pTarget; }
    eCOMPONENT_TYPE GetComponentType() { return m_Type; }
    IComponent* GetMyTargetComponent();

    //인자로 들어온 버퍼에 wstring 타입의 키값을 string 형태로 바꿔서 넣어주는 함수
    const std::string_view GetResKey(Ptr<IRes> _Res);


};
