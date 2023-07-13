#pragma once

#include <UtilLib_DLL/json/forwards.h>

class cEntity
{
public:
	cEntity();
	cEntity(const string_view _strName);


	cEntity(const cEntity& _other);
	virtual cEntity* Clone() = 0;

	virtual ~cEntity();

public:
	virtual bool SaveJson(Json::Value* _pJson);
	virtual bool LoadJson(Json::Value* _pJson);

private:
	static UINT32 g_iNextID;

private:
	const UINT32	m_iID;
public:
	UINT32 GetID() { return m_iID; }


private:
	string			m_strKey;
public:
	void SetKey(const string_view _strName) {  m_strKey = _strName; }
	const string& GetKey() { return m_strKey; }
};

