#include "pch.h"
#include "CUI_MeshRender.h"

#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CResMgr.h>

#include "CUI_ComboBox.h"

CUI_MeshRender::CUI_MeshRender()
	: CUI_Component("##CUI_MeshRender", eCOMPONENT_COLLIDER2D)
{
	SetName("MeshRenderUI");


	m_pComboBoxMesh = new CUI_ComboBox;
	AddChildUI(m_pComboBoxMesh);
	m_pComboBoxMesh->SetName("ComboBoxMesh");

	m_pComboBoxMtrl = new CUI_ComboBox;
	AddChildUI(m_pComboBoxMtrl);
	m_pComboBoxMtrl->SetName("ComboBoxMtrl");
}

CUI_MeshRender::~CUI_MeshRender()
{
}

void CUI_MeshRender::UpdateMeshListCallback()
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
void CUI_MeshRender::ChangeMeshCallback()
{
	const string& sel = m_pComboBoxMesh->GetCurrentSelected();
	if (true == sel.empty())
		return;

	Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(sel);

	if (nullptr == mesh)
		return;

	GetTarget()->MeshRender()->SetMesh(mesh);
}

void CUI_MeshRender::UpdateMtrlListCallback()
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

void CUI_MeshRender::ChangeMtrlCallback()
{
	const string& sel = m_pComboBoxMtrl->GetCurrentSelected();
	if (true == sel.empty())
		return;

	Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(sel);

	if (nullptr == mtrl)
		return;

	GetTarget()->MeshRender()->SetMaterial(mtrl);
}

void CUI_MeshRender::init()
{
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUI_MeshRender::UpdateMeshListCallback), eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMesh->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUI_MeshRender::ChangeMeshCallback)
	, eCALLBACK_TYPE::ONSELECT);

	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUI_MeshRender::UpdateMtrlListCallback)
		, eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMtrl->AddClickCallback(this, static_cast<UI_DELEGATE_0>(&CUI_MeshRender::ChangeMtrlCallback)
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

void CUI_MeshRender::tick()
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

int CUI_MeshRender::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;



	return TRUE;
}
