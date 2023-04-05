#pragma once

#include <UtilLib_DLL/json/forwards.h>

class CEntity
{
public:
	CEntity();
	CEntity(const string& _strName);


	CEntity(const CEntity& _other);
	virtual CEntity* Clone() = 0;

	virtual ~CEntity();

public:
	virtual bool Save(const std::filesystem::path _fileName) { return true; }
	virtual bool Load(const std::filesystem::path _fileName) { return true; }

	virtual bool SaveJson(Json::Value* _pJson);
	virtual bool LoadJson(Json::Value* _pJson);

private:
	static UINT32 g_iNextID;

private:
	string		m_strName;
	const UINT32  m_iID;

public:
	virtual void SetName(const string& _strName) {  m_strName = _strName; }
	const string& GetName() { return m_strName; }
	UINT32 GetID() { return m_iID; }


};

