#pragma once
#include "CUI_Widget.h"
#include "CImGuiMgr.h"

enum class eCALLBACK_TYPE
{
	ONCLICK,
	ONHOVER,
	ONSELECT,

	END
};

struct tComboItem
{
	string strName;
	tDataPtr pData;
};

class CUI_ComboBox 
	: public CUI_Widget
{
public:
	CUI_ComboBox() = delete;
	CUI_ComboBox(const string& _Name);
	virtual ~CUI_ComboBox();

public:
	virtual void tick() override;

	virtual bool beginUI() override;
	virtual void render_update() override;
	virtual void endUI() override;

private:
	vector<tComboItem> m_vecItem;
	int m_iCurrentSelected;
	ImGuiComboFlags m_ComboFlags;

	//함수 호출 대상
	vector<std::function<void(const tComboItem&)>> m_vecCallback[(UINT)eCALLBACK_TYPE::END];


public://Setter/Getter
	void SetImguiComboFlag(ImGuiComboFlags _Flags) { m_ComboFlags = _Flags; }

	void AddItem(const tComboItem& _tItem) { m_vecItem.push_back(_tItem); }

	//새로 들어온 벡터값으로 값을 교체
	void SetItem(vector<tComboItem>& _vecItem) { m_vecItem.clear(); std::swap(m_vecItem, _vecItem); }

	size_t GetItemNum() const { return m_vecItem.size(); }

	
	int	GetCurrentIndex() const { return m_iCurrentSelected; }
	void SetCurrentSelected(const string& _SelectedName);
	const tComboItem& GetCurrentSelected() const;

	void AddClickCallback(CUI_BasicWindow* _pInst, UI_DELEGATE_0 _pCallbackfunc, eCALLBACK_TYPE _Type);
	void ClearClickCallback(eCALLBACK_TYPE _Type);

private:
	bool IsIndexValid() const;
	void CallCallbackFunc(eCALLBACK_TYPE _Type);
};

inline const tComboItem& CUI_ComboBox::GetCurrentSelected() const
{
	static tComboItem voidItem = {};

	if (true == IsIndexValid())
		return m_vecItem[m_iCurrentSelected];
	else
		return voidItem;
}

inline void CUI_ComboBox::AddClickCallback(CUI_BasicWindow* _pInst, UI_DELEGATE_0 _pCallbackfunc, eCALLBACK_TYPE _Type)
{
	m_vecCallback[(UINT)_Type].push_back(std::bind(_pCallbackfunc, _pInst));
}

inline void CUI_ComboBox::ClearClickCallback(eCALLBACK_TYPE _Type)
{
	m_vecCallback[(UINT)_Type].clear();
}

inline bool CUI_ComboBox::IsIndexValid() const
{
	return ( 0 <= m_iCurrentSelected && m_iCurrentSelected < m_vecItem.size() );
}

inline void CUI_ComboBox::CallCallbackFunc(eCALLBACK_TYPE _Type)
{
	size_t size = m_vecCallback[(UINT)_Type].size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecCallback[(UINT)_Type][i](GetCurrentSelected());
	}
}

