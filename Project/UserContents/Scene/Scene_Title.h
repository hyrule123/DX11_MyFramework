#pragma once
#include <Engine/Game/iScene.h>

namespace ehw
{
	class Scene_Title : public iScene
	{
	public:
		Scene_Title();
		virtual ~Scene_Title();

		virtual void OnEnter() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnExit() override;

	private:
	};
}
