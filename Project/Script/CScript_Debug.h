#pragma once
#include <Engine/CScript.h>

#include <Engine/ptr.h>
class CCS_SCMapLoader;

class CScript_Debug :
    public CScript
{
public:
    CScript_Debug();
    ~CScript_Debug();

    CLONE(CScript_Debug);

public:
    virtual void init() override;
    virtual void tick() override;

private:
    Ptr<CCS_SCMapLoader> m_pCS;

public:
    void SetCS(Ptr<CCS_SCMapLoader> _pCS);

};

