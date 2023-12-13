#pragma once
#include <Engine/Game/Component/iComponent.h>

namespace ehw
{
    class Com_TestComponent :
        public iComponent
    {
    public:
        Com_TestComponent();
        virtual ~Com_TestComponent();

        virtual void Init()  override;
        virtual void Awake()  override;
        virtual void OnEnable()  override;
        virtual void OnDisable()  override;
        virtual void Start()  override;
        virtual void Update()  override;
        virtual void InternalUpdate() override;
    };

}

