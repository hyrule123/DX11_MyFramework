#include "pch.h"
#include "CUI_Transform.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

CUI_Transform::CUI_Transform()
    : CUI_Component("##Transform", eCOMPONENT_TRANSFORM)
{
    SetName("Transform");
}

CUI_Transform::~CUI_Transform()
{
}

int CUI_Transform::render_update()
{
    //���� �̸��� �θ��Լ��� ������Ʈ�� ������ ��� false�� ����
    if (FALSE == CUI_Component::render_update())
        return FALSE;

    CTransform* pTransform = GetTarget()->Transform();
    assert(nullptr != pTransform);

    //���� ������ �����ؾ� �ϹǷ� �Ϲ� value�� �޾ƿ´�.
    Vec3 vPos = pTransform->GetRelativePos();
    Vec3 vSize = pTransform->GetSize();
    Vec3 vScale = pTransform->GetRelativeScale();
    Vec3 vRot = (pTransform->GetRelativeRot() / XM_PI) * 180.f;


    //IMGUI�� �޾ƿ� ���� ǥ��
    ImGui::Text("Position");
    ImGui::SameLine(100.f);
    bool PosChanged = ImGui::DragFloat3("##Relative Position", vPos);


    ImGui::Text("Size");
    ImGui::SameLine(100.f);
    bool SizeChanged = ImGui::DragFloat3("##Size", vSize, 1.f, 0.f, FLT_MAX);


    ImGui::Text("Scale");
    ImGui::SameLine(100.f);
    bool ScaleChanged = ImGui::DragFloat3("##Relative Scale", vScale, 0.1f, 0.f, FLT_MAX);


    ImGui::Text("Rotation");
    ImGui::SameLine(100.f);
    bool RotationChanged = ImGui::DragFloat3("##Relative Rotation", vRot);
    


    //���� ����Ǿ��� ��� �ٽ� ���� ����.
    if (true == PosChanged)
        pTransform->SetRelativePos(vPos);

    else if (true == SizeChanged)
        pTransform->SetSize(vSize);

    else if (true == ScaleChanged)
        pTransform->SetRelativeScale(vScale);

    else if (true == RotationChanged)
    {
        vRot = (vRot / 180.f) * XM_PI;
        pTransform->SetRelativeRot(vRot);
    }

    return TRUE;
}
