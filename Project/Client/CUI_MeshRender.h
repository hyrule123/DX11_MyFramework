#pragma once
#include "CUI_Component.h"

class CUI_ComboBox;
class CUI_MeshRender :
    public CUI_Component
{
public:
    CUI_MeshRender();
    virtual ~CUI_MeshRender();

private:
    CUI_ComboBox* m_pComboBoxMesh;
    CUI_ComboBox* m_pComboBoxMtrl;

private:
    void UpdateMeshListCallback();
    void ChangeMeshCallback();

    void UpdateMtrlListCallback();
    void ChangeMtrlCallback();


public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

};