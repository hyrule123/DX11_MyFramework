#pragma once

#include "global.h"
#include "define.h"

#include "CComponent.h"


#include "ptr.h"
class CMesh;
class CMaterial;


class CRenderComponent :
    public CComponent
{

private:
    Ptr<CMesh>              m_pMesh;
    Ptr<CMaterial>          m_pMtrl;

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl) { m_pMtrl = _Mtrl; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
    bool GetRenderReady() { return ((nullptr != m_pMesh) && (nullptr != m_pMtrl)); }

public:
    virtual void render() = 0;

private:


public:
    CRenderComponent(eCOMPONENT_TYPE _type);
    virtual ~CRenderComponent();

};

