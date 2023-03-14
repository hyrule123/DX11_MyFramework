#pragma once
#include "CUIobj_Component.h"

class CUI_ComboBox;
class CUIobj_MeshRender :
    public CUIobj_Component
{
public:
    CUIobj_MeshRender();
    virtual ~CUIobj_MeshRender();

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
};
