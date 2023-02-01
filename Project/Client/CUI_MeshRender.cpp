#include "pch.h"
#include "CUI_MeshRender.h"

#include <Engine/CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CResMgr.h>

#include "CUI_ComboBox.h"

CUI_MeshRender::CUI_MeshRender()
	: CUI_Component("MeshRender", eCOMPONENT_COLLIDER2D)
{
	m_pComboBoxMesh = new CUI_ComboBox("MeshComboBox");
	AddChildUI(m_pComboBoxMesh);

	m_pComboBoxMtrl = new CUI_ComboBox("MtrlComboBox");
	AddChildUI(m_pComboBoxMtrl);
}

CUI_MeshRender::~CUI_MeshRender()
{
}

void CUI_MeshRender::UpdateMeshListCallback()
{
	//������ �޽��� ���� ��ü������ ���� ����� �����ؾ����� ���θ� �Ǵ��ϴ°��� ���� ����.
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

//�޽� �޺��ڽ����� Ư�� �޽��� Ŭ���Ǿ��� �� ȣ��� �Լ�
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
	//������ �޽��� ���� ��ü������ ���� ����� �����ؾ����� ���θ� �Ǵ��ϴ°��� ���� ����.
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

	//�ϴ� �⺻������ �޽��� ����Ʈ�� �̸��� �ѹ� �޾ƿ´�.
	UpdateMeshListCallback();
	UpdateMtrlListCallback();

	
	//Ÿ���� �����Ǿ� ������� Ÿ���� �޽��� ������ �̸��� �⺻ �������ش�.
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
	//���߿� ���ӿ�����Ʈ�� �����ؼ� �������� ��� �����ϸ� �Ʒ��ڵ�� �����,.
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
