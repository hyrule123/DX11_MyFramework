#pragma once
#include "IComponent.h"

class cCom_Transform;
class cCom_Camera;
class cCom_Renderer_Basic;
class ICollider;
class CFSM_Mgr;
class cFSM;

//한 게임오브젝트에서 활용하는 스크립트들 모음
//cFSM도 여기에서 담당
enum class eFSM_RESULT
{
    NULLPTR,
    ACCEPT,
    REJECT,
    RESERVE
};

class cScriptHolder 
    : public IComponent
{
public:
    cScriptHolder();

    cScriptHolder(const cScriptHolder& _other);
    CLONE(cScriptHolder);

    virtual ~cScriptHolder();

public:
    virtual bool SaveJson(Json::Value* _jVal);
    virtual bool LoadJson(Json::Value* _jVal);

public:
    virtual void init() final;
    virtual void start() final;
    virtual void tick() final;
    virtual void finaltick() final {}
    void BindData();
    void UnBind();
    virtual void cleanup() final {}

    eFSM_RESULT Transition(const tFSM_Event& _tEvent);
    eFSM_RESULT Transition(UINT _uStateID) { return Transition(tFSM_Event{ _uStateID, }); }
    eFSM_RESULT ForceTransition(const tFSM_Event& _tEvent);

private:
    //cFSM을 포함한 모든 스크립트를 들고있음(업데이트 용)
    vector<IScript*> m_vecScript;
    //map<std::string, IScript*> m_mapScript;

    //Index = cFSM의 인덱스 번호
    vector<cFSM*> m_vecFSM;
    cFSM* m_pCurrentFSM;

    tFSM_Event m_uReservedFSM;
private:
    bool CheckFSMValid(UINT _uStateID) const;
    void ResetReservedFSM() { m_uReservedFSM = tFSM_Event{}; }
    void ChangeFSM(const tFSM_Event& _tEvent);

public:
    bool AddScript(IScript* _pScript);
    IScript* FindScript(const string_view _strKey);

    const vector<IScript*>& GetScripts() const { return m_vecScript; }

    //이건 직접 호출할 필요 없음.(AddScript 할 시 알아서 호출 됨)
    bool AddFSM(cFSM* _pFSM);
    cFSM* GetCurFSM() const { return m_pCurrentFSM; }
    cFSM* GetFSM(UINT _uStateID) const { if (CheckFSMValid(_uStateID)) return m_vecFSM[_uStateID]; return nullptr; }
     
public:
    void BeginColiision(ICollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(ICollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(ICollider* _Other);
};

inline bool cScriptHolder::CheckFSMValid(UINT _uStateID) const
{
    if (_uStateID >= m_vecFSM.size())
        return false;
    else if (nullptr == m_vecFSM[_uStateID])
        return false;

    return true;
}






