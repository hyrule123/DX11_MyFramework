#pragma once

#include <UtilLib_DLL/json/forwards.h>

class IEntity
{
public:
	IEntity();
	IEntity(const string_view _strName);


	IEntity(const IEntity& _other);
	virtual IEntity* Clone() = 0;

	virtual ~IEntity();

public:
	virtual bool SaveJson(Json::Value* _pJson);
	virtual bool LoadJson(Json::Value* _pJson);

private:
	static UINT32	g_iNextID;

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

