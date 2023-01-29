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
	//������ �޽��� ���� ��ü������ ���� ����� �����ؾ����� ���θ� �Ǵ��ϴ°��� ���� ����.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MESH);
	if (m_pComboBoxMesh->GetItemNum() != map.size())
	{
		vector<string> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(::ConvertUnicodeToMultibyte(iter.second.Get()->GetKey()));
		}

		m_pComboBoxMesh->SetItem(tempVec);
	}

}

//�޽� �޺��ڽ����� Ư�� �޽��� Ŭ���Ǿ��� �� ȣ��� �Լ�
void CMeshRenderUI::ChangeMeshCallback()
{
	const wstring& sel = ::ConvertMultibyteToUnicode(m_pComboBoxMesh->GetCurrentSelected());
	if (true == sel.empty())
		return;

	Ptr<CMesh> mesh = CResMgr::GetInst()->FindRes<CMesh>(sel);

	if (nullptr == mesh)
		return;

	GetTarget()->MeshRender()->SetMesh(mesh);
}

void CMeshRenderUI::UpdateMtrlListCallback()
{
	//������ �޽��� ���� ��ü������ ���� ����� �����ؾ����� ���θ� �Ǵ��ϴ°��� ���� ����.
	const auto& map = CResMgr::GetInst()->GetResMap(eRES_TYPE::MATERIAL);
	if (m_pComboBoxMtrl->GetItemNum() != map.size())
	{
		vector<string> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(::ConvertUnicodeToMultibyte(iter.second.Get()->GetKey()));
		}

		m_pComboBoxMtrl->SetItem(tempVec);
	}
}

void CMeshRenderUI::ChangeMtrlCallback()
{
	const wstring& sel = ::ConvertMultibyteToUnicode(m_pComboBoxMtrl->GetCurrentSelected());
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

	//�ϴ� �⺻������ �޽��� ����Ʈ�� �̸��� �ѹ� �޾ƿ´�.
	UpdateMeshListCallback();
	UpdateMtrlListCallback();

	
	//Ÿ���� �����Ǿ� ������� Ÿ���� �޽��� ������ �̸��� �⺻ �������ش�.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		const wstring& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
		m_pComboBoxMesh->SetCurrentSelected(::ConvertUnicodeToMultibyte(meshname));


		const wstring& mtrlname = GetTarget()->MeshRender()->GetMaterial()->GetKey();
		m_pComboBoxMtrl->SetCurrentSelected(::ConvertUnicodeToMultibyte(mtrlname));
	}
}

void CMeshRenderUI::tick()
{
	//���߿� ���ӿ�����Ʈ�� �����ؼ� �������� ��� �����ϸ� �Ʒ��ڵ�� �����,.
	if (nullptr != GetTarget() && nullptr != GetTarget()->MeshRender())
	{
		if (-1 == m_pComboBoxMesh->GetCurrentIndex())
		{
			const wstring& meshname = GetTarget()->MeshRender()->GetMesh()->GetKey();
			m_pComboBoxMesh->SetCurrentSelected(::ConvertUnicodeToMultibyte(meshname));
		}

		if (-1 == m_pComboBoxMtrl->GetCurrentIndex())
		{
			const wstring& mtrlname = GetTarget()->MeshRender()->GetMaterial()->GetKey();
			m_pComboBoxMtrl->SetCurrentSelected(::ConvertUnicodeToMultibyte(mtrlname));
		}
	}

}

int CMeshRenderUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;



	return TRUE;
}
