#include "pch.h"
#include "CMeshRenderUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CResMgr.h>

#include "CComboBox.h"

CMeshRenderUI::CMeshRenderUI()
	: CComponentUI("##CMeshRenderUI", eCOMPONENT_COLLIDER2D)
{
	SetName("MeshRenderUI");


	m_pComboBoxMesh = new CComboBox;
	AddChildUI(m_pComboBoxMesh);
	m_pComboBoxMesh->SetName("ComboBoxMesh");

	m_pComboBoxMtrl = new CComboBox;
	AddChildUI(m_pComboBoxMtrl);
	m_pComboBoxMtrl->SetName("ComboBoxMtrl");
}

CMeshRenderUI::~CMeshRenderUI()
{
}

void CMeshRenderUI::UpdateMeshListCallback()
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MESH);
	if (m_pComboBoxMesh->GetItemNum() != map.size())
	{
		vector<string> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(iter.second.Get()->GetKey());
		}

		m_pComboBoxMesh->SetItem(tempVec);
	}

}

//메쉬 콤보박스에서 특정 메쉬가 클릭되었을 때 호출될 함수
void CMeshRenderUI::ChangeMeshCallback()
{
	const string& sel = m_pComboBoxMesh->GetCurrentSelected();
	if (true == sel.empty())
		return;

	Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(sel);

	if (nullptr == mesh)
		return;

	GetTarget()->MeshRender()->SetMesh(mesh);
}

void CMeshRenderUI::UpdateMtrlListCallback()
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MATERIAL);
	if (m_pComboBoxMtrl->GetItemNum() != map.size())
	{
		vector<string> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(iter.second.Get()->GetKey());
		}

		m_pComboBoxMtrl->SetItem(tempVec);
	}
}

void CMeshRenderUI::ChangeMtrlCallback()
{
	const string& sel = m_pComboBoxMtrl->GetCurrentSelected();
	if (true == sel.empty())
		return;

	Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(sel);

	if (nullptr == mtrl)
		return;

	GetTarget()->MeshRender()->SetMaterial(mtrl);
}

void CMeshRenderUI::init()
{
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CMeshRenderUI::UpdateMeshListCallback), eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CMeshRenderUI::ChangeMeshCallback)
	, eCALLBACK_TYPE::ONSELECT);

	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CMeshRenderUI::UpdateMtrlListCallback)
		, eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CMeshRenderUI::ChangeMtrlCallback)
		, eCALLBACK_TYPE::ONSELECT);

	//일단 기본적으로 메쉬와 리스트의 이름을 한번 받아온다.
	UpdateMeshListCallback();
	UpdateMtrlListCallback();

	
	//타겟이 지정되어 있을경우 타겟의 메쉬와 재질의 이름을 기본 세팅해준다.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		const string& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
		m_pComboBoxMesh->SetCurrentSelected(meshname);


		const string& mtrlname = GetTarget()->MeshRender()->GetMaterial()->GetKey();
		m_pComboBoxMtrl->SetCurrentSelected(meshname);
	}
}

void CMeshRenderUI::tick()
{
	//나중에 게임오브젝트를 선택해서 가져오는 기능 구현하면 아래코드는 지울것,.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		if (-1 == m_pComboBoxMesh->GetCurrentIndex())
		{
			const string& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
			m_pComboBoxMesh->SetCurrentSelected(meshname);
		}

		if (-1 == m_pComboBoxMtrl->GetCurrentIndex())
		{
			const string& mtrlname = GetTarget()->MeshRender()->GetMaterial()->GetKey();
			m_pComboBoxMtrl->SetCurrentSelected(mtrlname);
		}
	}

}

int CMeshRenderUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;



	return TRUE;
}
