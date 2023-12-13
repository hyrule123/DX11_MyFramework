#pragma once
#include <Engine/Game/Component/Script/iScript.h>

namespace ehw
{
    class Script_Test :
        public iScript
    {
    public:
        Script_Test();
        virtual ~Script_Test();

        void Update() override;

    };
}


