#pragma once



class ManualEdit
{
public:
	static void Edit();
	

private:
	static void Terran_CommonAnim_Save();

	//Marine
	static void MarineAnim_Save(const string& _strKey);
	static void MarinePrefab_Save(const string& _strKey);

	//Command Center
	static void CommandCenter_Anim_Save(const string& _strKey);
	static void CommandCenter_Prefab_Save(const string& _strKey);
	

	static void LoadAnim(const string& _strKey);
	static void LoadPrefab(const string& _strKey);
};


