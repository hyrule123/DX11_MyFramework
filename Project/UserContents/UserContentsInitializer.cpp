#include "PCH_UserContents.h"
#include "UserContentsInitializer.h"

#include "strKey_Scene.h"
#include <EngineBase/Engine/SceneMgr.h>

namespace ehw
{
	void UserContentsInitializer::Init()
	{
		InitUserComponent();
		InitScript();
		InitScene();

		InitUserContents();
	}


	//여기에 게임 초기화 코드를 작성
	//Contents 프로젝트에서 건드릴수 있는 부분은 여기만임. 나머지는 터치 x
	void UserContentsInitializer::InitUserContents()
	{
		//예시 씬을 첫 시작으로 등록
		SceneMgr::LoadScene(strKey::Scene::Scene_Title);
	}
}
