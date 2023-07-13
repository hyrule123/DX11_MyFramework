#pragma once

#include <UtilLib_DLL/json/forwards.h>

class Entity
{
public:
	Entity();
	Entity(const string_view _strName);


	Entity(const Entity& _other);
	virtual Entity* Clone() = 0;

	virtual ~Entity();

public:
	virtual bool Save(const std::filesystem::path& _fileName) { return false; }
	virtual bool Load(const std::filesystem::path& _fileName) { return false; }

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

