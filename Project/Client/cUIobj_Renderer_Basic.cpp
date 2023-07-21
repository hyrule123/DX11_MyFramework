#include "pch.h"
#include "cUIobj_Renderer_Basic.h"

#include <Engine/cGameObject.h>
#include <Engine/cCom_Renderer_Default.h>
#include <Engine/cResMgr.h>

#include "cUI_ComboBox.h"

cUIobj_Renderer_Basic::cUIobj_Renderer_Basic()
	: cUIobj_Component("MeshRender", eCOMPONENT_TYPE::RENDERER)
{
	m_pComboBoxMesh = new cUI_ComboBox("MeshComboBox");
	AddChildUI(m_pComboBoxMesh);

	m_pComboBoxMtrl = new cUI_ComboBox("MtrlComboBox");
	AddChildUI(m_pComboBoxMtrl);
}

cUIobj_Renderer_Basic::~cUIobj_Renderer_Basic()
{
}

void cUIobj_Renderer_Basic::UpdateMeshListCallback(const tComboItem& _tComboItem)
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = cResMgr::GetInst()->GetResMap(eRES_TYPE::MESH);
	if (m_pComboBoxMesh->GetItemNum() != map.size())
	{
		vector<tComboItem> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(tComboItem{ iter.second.Get()->GetKey(), iter.second.Get(), });
		}

		m_pComboBoxMesh->SetItem(tempVec);
	}

}

//메쉬 콤보박스에서 특정 메쉬가 클릭되었을 때 호출될 함수
void cUIobj_Renderer_Basic::ChangeMeshCallback(const tComboItem& _tComboItem)
{
	const tComboItem& sel = m_pComboBoxMesh->GetCurrentSelected();
	if (true == sel.strName.empty())
		return;

	if (nullptr == sel.pData.ptr)
		return;

	GetTargetObj()->Renderer()->SetMesh(static_cast<cMesh*>(sel.pData.ptr));
}

void cUIobj_Renderer_Basic::UpdateMtrlListCallback(const tComboItem& _tComboItem)
{
	//원래는 메쉬의 갯수 자체만으로 새로 목록을 갱신해야할지 여부를 판단하는것은 좋지 않음.
	const auto& map = cResMgr::GetInst()->GetResMap(eRES_TYPE::MATERIAL);
	if (m_pComboBoxMtrl->GetItemNum() != map.size())
	{
		vector<tComboItem> tempVec;
		for (const auto& iter : map)
		{
			tempVec.push_back(tComboItem{ iter.second.Get()->GetKey(), iter.second.Get(), });
		}

		m_pComboBoxMtrl->SetItem(tempVec);
	}
}

void cUIobj_Renderer_Basic::ChangeMtrlCallback(const tComboItem& _tComboItem)
{
	
	if (true == _tComboItem.strName.empty())
		return;

	Ptr<cMaterial> mtrl = static_cast<cMaterial*>(_tComboItem.pData.ptr);

	if (nullptr == mtrl)
		return;

	GetTargetObj()->Renderer()->SetMaterial(mtrl);
}

void cUIobj_Renderer_Basic::init()
{
	m_pComboBoxMesh->AddClickCallback(std::bind(&cUIobj_Renderer_Basic::UpdateMeshListCallback, this, std::placeholders::_1), eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMesh->AddClickCallback(std::bind(&cUIobj_Renderer_Basic::ChangeMeshCallback, this, std::placeholders::_1)
	, eCALLBACK_TYPE::ONSELECT);

	m_pComboBoxMtrl->AddClickCallback(std::bind(&cUIobj_Renderer_Basic::UpdateMeshListCallback, this, std::placeholders::_1)
		, eCALLBACK_TYPE::ONCLICK);
	m_pComboBoxMtrl->AddClickCallback(std::bind(&cUIobj_Renderer_Basic::ChangeMtrlCallback, this, std::placeholders::_1)
		, eCALLBACK_TYPE::ONSELECT);

	//일단 기본적으로 메쉬와 리스트의 이름을 한번 받아온다.
	UpdateMeshListCallback(tComboItem{});
	UpdateMtrlListCallback(tComboItem{});

	
	//타겟이 지정되어 있을경우 타겟의 메쉬와 재질의 이름을 기본 세팅해준다.
	if (nullptr != GetTargetObj())
	{
		cCom_Renderer_Default* Renderer = GetTargetObj()->GetComponent<cCom_Renderer_Default>();
		if (nullptr == Renderer)
			return;

		const string& meshname = Renderer->GetMesh()->GetKey();
		m_pComboBoxMesh->SetCurrentSelected(meshname);


		const string& mtrlname = Renderer->GetCurMaterial()->GetKey();
		m_pComboBoxMtrl->SetCurrentSelected(meshname);
	}
}

void cUIobj_Renderer_Basic::Tick()
{
	//나중에 게임오브젝트를 선택해서 가져오는 기능 구현하면 아래코드는 지울것,.
	if (nullptr != GetTargetObj() && nullptr != GetTargetObj()->Renderer())
	{
		if (-1 == m_pComboBoxMesh->GetCurrentIndex())
		{
			const string& meshname = GetTargetObj()->Renderer()->GetMesh()->GetKey();
			m_pComboBoxMesh->SetCurrentSelected(meshname);
		}

		if (-1 == m_pComboBoxMtrl->GetCurrentIndex())
		{
			const string& mtrlname = GetTargetObj()->Renderer()->GetCurMaterial()->GetKey();
			m_pComboBoxMtrl->SetCurrentSelected(mtrlname);
		}
	}

}
