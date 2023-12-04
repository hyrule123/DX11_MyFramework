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

        virtual void Init()  override;
        virtual void Awake()  override;
        virtual void OnEnable()  override;
        virtual void OnDisable()  override;
        virtual void Start()  override;
        virtual void Update()  override;
        virtual void OnDestroy()  override;
        virtual void InternalUpdate() override;
    };

}

