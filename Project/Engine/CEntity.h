#pragma once

class CEntity
{
private:
	static UINT32 g_iNextID;

private:
	wstring		m_strName;
	const UINT32  m_iID;

public:
	void SetName(const wstring& _strName) {  m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT32 GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
};

