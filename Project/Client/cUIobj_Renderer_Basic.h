#pragma once
#include "cUIobj_Component.h"

#include "cUI_Structs.h"

class cUI_ComboBox;
class cUIobj_Renderer_Basic :
    public cUIobj_Component
{
public:
    cUIobj_Renderer_Basic();
    virtual ~cUIobj_Renderer_Basic();

private:
    cUI_ComboBox* m_pComboBoxMesh;
    cUI_ComboBox* m_pComboBoxMtrl;

private:
    void UpdateMeshListCallback(const tComboItem& _tComboItem);
    void ChangeMeshCallback(const tComboItem& _tComboItem);

    void UpdateMtrlListCallback(const tComboItem& _tComboItem);
    void ChangeMtrlCallback(const tComboItem& _tComboItem);


public:
    virtual void init() override;
    virtual void Tick() override;
};
