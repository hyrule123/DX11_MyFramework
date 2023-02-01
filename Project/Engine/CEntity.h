#pragma once

class CEntity
{
private:
	static UINT32 g_iNextID;

private:
	string		m_strName;
	const UINT32  m_iID;

public:
	void SetName(const string& _strName) {  m_strName = _strName; }
	const string& GetName() { return m_strName; }
	UINT32 GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
};

