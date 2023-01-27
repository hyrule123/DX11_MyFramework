#include "pch.h"
#include "CTransformUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

CTransformUI::CTransformUI()
    : CComponentUI("##Transform", eCOMPONENT_TRANSFORM)
{
    SetName("Transform");
}

CTransformUI::~CTransformUI()
{
}

int CTransformUI::render_update()
{
    //같은 이름의 부모함수의 업데이트가 실패할 경우 false를 리턴
    if (FALSE == CComponentUI::render_update())
        return FALSE;

    CTransform* pTransform = GetTarget()->Transform();
    assert(nullptr != pTransform);

    //값을 수정도 가능해야 하므로 일반 value
    Vec3 vPos = pTransform->GetRelativePos();
    Vec3 vSize = pTransform->GetSize();
    Vec3 vScale = pTransform->GetRelativeScale();
    Vec3 vRot = (pTransform->GetRelativeRot() / XM_PI) * 180.f;


    //IMGUI에 받아온 값을 표시
    ImGui::Text("Position");
    ImGui::SameLine();
    bool PosChanged = ImGui::DragFloat3("##Relative Position", vPos);

    ImGui::Text("Size    ");
    ImGui::SameLine();
    bool SizeChanged = ImGui::DragFloat3("##Size", vSize, 1.f, 0.f, FLT_MAX);

    ImGui::Text("Scale   ");
    ImGui::SameLine();
    bool ScaleChanged = ImGui::DragFloat3("##Relative Scale", vScale, 0.1f, 0.f, FLT_MAX);

    ImGui::Text("Rotation");
    ImGui::SameLine();
    bool RotationChanged = ImGui::DragFloat3("##Relative Rotation", vRot);


    //값이 변경되었을 경우 다시 값을 설정.
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
