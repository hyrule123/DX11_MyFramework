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


public:
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

