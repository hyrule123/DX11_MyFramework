#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;

class CGameObject :
    public CEntity
{
private:
    CComponent*         m_arrCom[(UINT)eCOMPONENT_TYPE::END];

    //초기화 되어 현재 Level 안에서 작동중인지 여부를 저장.
    //작동 이후 컴포넌트가 추가될 시 바로 init 호출.
    bool                m_initalized;


public:
    void init();
    void tick();
    void finaltick();


    void render();


public:
    void AddComponent(CComponent* _Component);

    CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM]; }
    CMeshRender* MeshRender() { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::MESHRENDER]; }



    CLONE(CGameObject)    
public:
    CGameObject();
    ~CGameObject();

};

