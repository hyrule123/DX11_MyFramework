#pragma once
#include "CUI_BasicWindow.h"

#include <Engine/CRes.h>
#include <Engine/ptr.h>

class CGameObject;

class CUI_Component :
    public CUI_BasicWindow
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

    //���ڷ� ���� ���ۿ� wstring Ÿ���� Ű���� string ���·� �ٲ㼭 �־��ִ� �Լ�
    const string& GetResKey(Ptr<CRes> _Res);

public:
    virtual void tick() override;

    virtual bool beginUI() override;
    virtual void render_update() override;
    virtual void endUI() override;


};