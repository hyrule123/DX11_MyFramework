#pragma once
#include "Component.h"

class Transform;
class Camera;
class MeshRenderer;
class Collider;
class CFSM_Mgr;
class FSM;

//한 게임오브젝트에서 활용하는 스크립트들 모음
//FSM도 여기에서 담당
enum class eFSM_RESULT
{
    NULLPTR,
    ACCEPT,
    REJECT,
    RESERVE
};

class ScriptHolder 
    : public Component
{
public:
    ScriptHolder();

    ScriptHolder(const ScriptHolder& _other);
    CLONE(ScriptHolder);

    virtual ~ScriptHolder();

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
    //FSM을 포함한 모든 스크립트를 들고있음(업데이트 용)
    vector<Script*> m_vecScript;
    //map<std::string, Script*> m_mapScript;

    //Index = FSM의 인덱스 번호
    vector<FSM*> m_vecFSM;
    FSM* m_pCurrentFSM;

    tFSM_Event m_uReservedFSM;
private:
    bool CheckFSMValid(UINT _uStateID) const;
    void ResetReservedFSM() { m_uReservedFSM = tFSM_Event{}; }
    void ChangeFSM(const tFSM_Event& _tEvent);

public:
    bool AddScript(Script* _pScript);
    Script* FindScript(const string_view _strKey);

    const vector<Script*>& GetScripts() const { return m_vecScript; }

    //이건 직접 호출할 필요 없음.(AddScript 할 시 알아서 호출 됨)
    bool AddFSM(FSM* _pFSM);
    FSM* GetCurFSM() const { return m_pCurrentFSM; }
    FSM* GetFSM(UINT _uStateID) const { if (CheckFSMValid(_uStateID)) return m_vecFSM[_uStateID]; return nullptr; }
     
public:
    void BeginColiision(Collider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(Collider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(Collider* _Other);
};

inline bool ScriptHolder::CheckFSMValid(UINT _uStateID) const
{
    if (_uStateID >= m_vecFSM.size())
        return false;
    else if (nullptr == m_vecFSM[_uStateID])
        return false;

    return true;
}






