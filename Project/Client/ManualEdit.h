#pragma once

class ManualEdit
{
public:
	static void Edit();
	

private:
	static void MarineAnim_Save(const string& _strKey);
	static void MarinePrefab_Save(const string& _strKey);

	static void LoadAnim(const string& _strKey);
	static void LoadPrefab(const string& _strKey);
};


