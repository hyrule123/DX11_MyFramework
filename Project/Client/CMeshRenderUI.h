#pragma once
#include "CComponentUI.h"

class CComboBox;
class CMeshRenderUI :
    public CComponentUI
{
public:
    CMeshRenderUI();
    virtual ~CMeshRenderUI();

private:
    CComboBox* m_pComboBoxMesh;
    CComboBox* m_pComboBoxMtrl;

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