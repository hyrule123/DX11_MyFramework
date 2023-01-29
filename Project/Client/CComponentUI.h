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

    //���ڷ� ���� ���ۿ� wstring Ÿ���� Ű���� string ���·� �ٲ㼭 �־��ִ� �Լ�
    bool GetResKey(Ptr<CRes> _Res, __out string& _out);

public:
    virtual void tick() override;
    virtual int render_update() override;



};