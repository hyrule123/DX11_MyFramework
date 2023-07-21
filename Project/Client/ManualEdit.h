#pragma once

#include <Engine/Ptr.h>

class cPrefab;
class cAnim2DAtlas;

class ManualEdit
{
public:
	static void Edit();
	static void TestCreate();
	

private:
	static void Terran_CommonAnim_Save();

	//Marine
	static void MarineAnim_Save(const string& _strKey);
	static void MarinePrefab_Save(const string& _strKey);

	//Command Center
	static void CommandCenter_Anim_Save();
	static void CommandCenter_Prefab_Save(const string& _strKey);

	static void Mineral_Prefab_Save();

	static void Vespene_Anim_Save();
	static void Vespene_Prefab_Save();

	static void Map_Prefab_Save();

	static Ptr<cAnim2DAtlas> LoadAnim(const string& _strKey);
	static Ptr<cPrefab> LoadPrefab(const string& _strKey);


};


