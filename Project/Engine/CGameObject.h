#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CCamera;

class CGameObject :
    public CEntity
{
private:
    CComponent*         m_arrCom[(UINT)eCOMPONENT_TYPE::END];

    //�ʱ�ȭ �Ǿ� ���� Level �ȿ��� �۵������� ���θ� ����.
    //�۵� ���� ������Ʈ�� �߰��� �� �ٷ� init ȣ��.
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
    CCamera* Camera() { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }



    CLONE(CGameObject)    
public:
    CGameObject();
    ~CGameObject();

};

