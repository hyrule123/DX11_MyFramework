#pragma once
#include "CComponent.h"


#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"
#include "ptr.h"


class CMeshRender :
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

public:
    virtual void finaltick() override;
    void render();

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

