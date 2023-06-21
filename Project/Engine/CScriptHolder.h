#pragma once
#include "CComponent.h"

class CTransform;
class CCamera;
class CMeshRender;
class CCollider;
class CFSM_Mgr;
class CFSM;

//한 게임오브젝트에서 활용하는 스크립트들 모음
//FSM도 여기에서 담당
enum class eFSM_RESULT
{
    NULLPTR,
    ACCEPT,
    REJECT,
    RESERVE
};

class CScriptHolder 
    : public CComponent
{
public:
    CScriptHolder();

    CScriptHolder(const CScriptHolder& _other);
    CLONE(CScriptHolder);

    virtual ~CScriptHolder();

public:
    virtual bool SaveJson(Json::Value* _jVal);
    virtual bool LoadJson(Json::Value* _jVal);

public:
    virtual void init() final;
    virtual void start() final;
    virtual void tick() final;
    virtual void finaltick() final {}
    virtual void cleanup() final {}

    eFSM_RESULT Transition(const tFSM_Event& _tEvent);
    eFSM_RESULT Transition(UINT _uStateID) { return Transition(tFSM_Event{ _uStateID, }); }
    eFSM_RESULT ForceTransition(const tFSM_Event& _tEvent);

private:
    //FSM을 포함한 모든 스크립트를 들고있음(업데이트 용)
    vector<CScript*> m_vecScript;
    //map<std::string, CScript*> m_mapScript;

    //Index = FSM의 인덱스 번호
    vector<CFSM*> m_vecFSM;
    CFSM* m_pCurrentFSM;

    tFSM_Event m_uReservedFSM;
private:
    bool CheckFSMValid(UINT _uStateID) const;
    void ResetReservedFSM() { m_uReservedFSM = tFSM_Event{}; }
    void ChangeFSM(const tFSM_Event& _tEvent);

public:
    bool AddScript(CScript* _pScript);
    CScript* FindScript(const string_view _strKey);

    const vector<CScript*>& GetScripts() const { return m_vecScript; }

    //이건 직접 호출할 필요 없음.(AddScript 할 시 알아서 호출 됨)
    bool AddFSM(CFSM* _pFSM);
    CFSM* GetCurFSM() const { return m_pCurrentFSM; }
    CFSM* GetFSM(UINT _uStateID) const { if (CheckFSMValid(_uStateID)) return m_vecFSM[_uStateID]; return nullptr; }
     
public:
    void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(CCollider* _Other);
};

inline bool CScriptHolder::CheckFSMValid(UINT _uStateID) const
{
    if (_uStateID >= m_vecFSM.size())
        return false;
    else if (nullptr == m_vecFSM[_uStateID])
        return false;

    return true;
}






