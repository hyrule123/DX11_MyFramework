#include "PCH_UserContents.h"
#include "Com_TestComponent.h"

#include <EngineBase/Engine/InputMgr.h>
#include <EngineBase/Engine/GameObject.h>

namespace ehw
{
	Com_TestComponent::Com_TestComponent()
		:IComponent(eComponentCategory::Animator)
	{
	}
	Com_TestComponent::~Com_TestComponent()
	{
	}
	void Com_TestComponent::Init()
	{
		int a = 3;
	}
	void Com_TestComponent::Awake()
	{
		int a = 3;
	}
	void Com_TestComponent::OnEnable()
	{
		int a = 3;
	}

	void Com_TestComponent::OnDisable()
	{
		int a = 3;
	}
	void Com_TestComponent::Start()
	{
		int a = 3;
	}
	void Com_TestComponent::Update()
	{
		int a = 3;

		if (InputMgr::GetKeyDown(eKeyCode::E))
		{
			GetOwner()->SetActive(false);
		}
	}
	void Com_TestComponent::OnDestroy()
	{
		int a = 3;
	}
	void Com_TestComponent::InternalUpdate()
	{
		int a = 3;
	}
}
