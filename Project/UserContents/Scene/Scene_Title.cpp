#include "Scene_Title.h"

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Game/Component/Transform/Com_Transform.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Mesh.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Game/Component/Camera/Com_Camera.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_Sprite.h>
#include <Engine/Game/Component/Script/GridScript.h>
#include <Engine/Manager/InputManager.h>
#include <Engine/Game/Component/Collider/ICollider2D.h>
#include <Engine/Manager/CollisionManager.h>
#include <Engine/Game/Component/Animator/Com_Animator2D.h>
#include <Engine/Game/Component/Light/Com_Light3D.h>
#include <Engine/Game/Component/Animator/Com_Animator3D.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_ParticleSystem.h>
#include <Engine/Resource/Prefab.h>
#include <Engine/Game/Component/Renderer/Com_Renderer_UIBase.h>

#include "../strKey_Script.h"
#include "../strKey_UserComponent.h"


namespace ehw
{
	Scene_Title::Scene_Title()
	{
	}
	Scene_Title::~Scene_Title()
	{
	}
	void Scene_Title::OnEnter()
	{

		{
			// Main Com_Camera Game Object
			GameObject* cameraObj = NewGameObject(eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");

			Com_Transform* tr = cameraObj->AddComponent<Com_Transform>();
			tr->SetRelativePos(float3(0.0f, 0.0f, -20.0f));

			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(eProjectionType::Perspective);

			//cameraObj->AddComponent(strKey::Script::Script_CameraMove);
			//cameraObj->AddComponent(strKey::Script::Script_UIBase);


			RenderManager::SetMainCamera(cameraComp);
		}

		{
			GameObject* dirLight = NewGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Directional);
			light3d->SetDiffuse(float4(0.3f, 0.3f, 0.3f, 1.f));
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}

		{
			GameObject* dirLight = NewGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();
			dirLight->SetName("Point1000");

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Point);
			light3d->SetRadius(1000.f);
			light3d->SetDiffuse(float4(0.3f, 0.3f, 0.3f, 1.f));
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}

		{
			GameObject* dirLight = NewGameObject(eLayerType::Player);
			dirLight->AddComponent<Com_Transform>();
			dirLight->SetName("Point500");

			Com_Light3D* light3d = dirLight->AddComponent<Com_Light3D>();
			light3d->SetLightType(eLightType::Point);
			light3d->SetRadius(500.f);

			light3d->SetDiffuse(float4(0.7f, 0.7f, 0.7f, 1.f));
			
			light3d->SetAmbient(float4(0.3f, 0.3f, 0.3f, 1.f));
		}


		{
			GameObject* player = NewGameObject(eLayerType::Player);
			player->SetName("TestObj");

			player->AddComponent(strKey::component::Com_TestComponent);
			//player->AddComponent()
			//player->AddComponent<Script_Player>();

			
			//GameObject* modeling = meshdata->Instantiate(eLayerType::Player);
		}
	}
	void Scene_Title::Update()
	{
		iScene::Update();
	}

	void Scene_Title::Render()
	{
		iScene::Render();
	}

	void Scene_Title::OnExit()
	{
	}
}
