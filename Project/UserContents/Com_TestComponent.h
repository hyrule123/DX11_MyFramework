#pragma once
#include <EngineBase/Engine/IComponent.h>

namespace ehw
{
    class Com_TestComponent :
        public IComponent
    {
    public:
        Com_TestComponent();
        virtual ~Com_TestComponent();

        void FixedUpdate() override {}
    };

}

