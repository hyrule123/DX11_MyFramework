#pragma once
#include "CUIobj_Component.h"

#include "CUI_Structs.h"

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
    void UpdateMeshListCallback(const tComboItem& _tComboItem);
    void ChangeMeshCallback(const tComboItem& _tComboItem);

    void UpdateMtrlListCallback(const tComboItem& _tComboItem);
    void ChangeMtrlCallback(const tComboItem& _tComboItem);


public:
    virtual void init() override;
    virtual void tick() override;
};
