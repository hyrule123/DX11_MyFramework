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


class CUI_ComboBox 
	: public CUI_Widget
{
public:
	CUI_ComboBox();
	virtual ~CUI_ComboBox();

private:
	vector<string> m_vecItem;
	int m_iCurrentSelected;
	ImGuiComboFlags m_ComboFlags;

	//함수 호출 대상
	vector<std::function<void()>> m_vecCallback[(UINT)eCALLBACK_TYPE::END];


public://Setter/Getter
	void SetImguiComboFlag(ImGuiComboFlags _Flags) { m_ComboFlags = _Flags; }

	void AddItem(const string& _str) { m_vecItem.push_back(_str); }

	//새로 들어온 벡터값으로 값을 교체
	void SetItem(vector<string>& _vecItem) { m_vecItem.clear(); std::swap(m_vecItem, _vecItem); }

	size_t GetItemNum() const { return m_vecItem.size(); }

	
	int	GetCurrentIndex() const { return m_iCurrentSelected; }
	void SetCurrentSelected(const string& _SelectedName);
	const string& GetCurrentSelected() const;

	void AddClickCallback(CUI* _pInst, UI_DELEGATE_0 _pCallbackfunc, eCALLBACK_TYPE _Type);
	void ClearClickCallback(eCALLBACK_TYPE _Type);

private:
	bool IsIndexValid() const;
	void CallCallbackFunc(eCALLBACK_TYPE _Type);

public:
	virtual void tick() override;
	virtual int render_update() override;



};

inline const string& CUI_ComboBox::GetCurrentSelected() const
{
	if (true == IsIndexValid())
		return m_vecItem[m_iCurrentSelected];
	else
		return g_voidStr;
}

inline void CUI_ComboBox::AddClickCallback(CUI* _pInst, UI_DELEGATE_0 _pCallbackfunc, eCALLBACK_TYPE _Type)
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
		m_vecCallback[(UINT)_Type][i]();
	}
}

