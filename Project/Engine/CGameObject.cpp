#include "pch.h"
#include "CGameObject.h"

#include "components.h"

//레이어 옮길 때 사용
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//트랜스폼 상속
#include "CTransform.h"

//생명주기 계산용
#include "CTimeMgr.h"

#include "CTransform.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_bInitialized()
	, m_RenderCom()
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_bFixLayer()
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
}

CGameObject::CGameObject(const CGameObject& _other)
	: CEntity(_other)
	, m_iLayerIdx(_other.m_iLayerIdx)
	, m_bFixLayer(_other.m_bFixLayer)
	, m_bInitialized(_other.m_bInitialized)
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEG)
{
	//1. 컴포넌트 목록 복사
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _other.m_arrCom[i])
		{
			m_arrCom[i] = _other.m_arrCom[i]->Clone();
			m_arrCom[i]->SetOwner(this);

			//1-1. 렌더링 컴포넌트 일 경우 m_RenderCom에 복사
			if ((UINT)g_RenderComIdxStart <= i && i < (UINT)g_RenderComIdxEnd)
			{
				assert(nullptr == m_RenderCom);
				m_RenderCom = static_cast<CRenderComponent*>(m_arrCom[i]);
			}
		}
	}

	//2. 자녀 오브젝트 복사
	size_t size = _other.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddChildObj(_other.m_vecChild[i]->Clone());
	}

}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}

void CGameObject::SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
{
	switch (_Param)
	{
	case eMTRLDATA_PARAM_SCALAR::INT_0: m_MtrlScalarData.INT_0 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_1: m_MtrlScalarData.INT_1 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_2: m_MtrlScalarData.INT_2 = *((int*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::INT_3: m_MtrlScalarData.INT_3 = *((int*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::FLOAT_0: m_MtrlScalarData.FLOAT_0 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_1: m_MtrlScalarData.FLOAT_1 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_2: m_MtrlScalarData.FLOAT_2 = *((float*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::FLOAT_3: m_MtrlScalarData.FLOAT_3 = *((float*)_Src);
		break;


	case eMTRLDATA_PARAM_SCALAR::VEC2_0: m_MtrlScalarData.VEC2_0 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_1: m_MtrlScalarData.VEC2_1 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_2: m_MtrlScalarData.VEC2_2 = *((Vec2*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC2_3: m_MtrlScalarData.VEC2_3 = *((Vec2*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::VEC4_0: m_MtrlScalarData.VEC4_0 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_1: m_MtrlScalarData.VEC4_1 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_2: m_MtrlScalarData.VEC4_2 = *((Vec4*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::VEC4_3: m_MtrlScalarData.VEC4_3 = *((Vec4*)_Src);
		break;

	case eMTRLDATA_PARAM_SCALAR::MAT_0: 
		memcpy(m_MtrlScalarData.MAT_0.m, _Src, sizeof(Matrix));
		//m_MtrlScalarData.MAT_0 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_1: 
		memcpy(m_MtrlScalarData.MAT_1.m, _Src, sizeof(Matrix));
		//m_MtrlScalarData.MAT_1 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_2: 
		memcpy(m_MtrlScalarData.MAT_2.m, _Src, sizeof(Matrix));
		//m_MtrlScalarData.MAT_2 = *((Matrix*)_Src);
		break;
	case eMTRLDATA_PARAM_SCALAR::MAT_3: 
		memcpy(m_MtrlScalarData.MAT_3.m, _Src, sizeof(Matrix));
		//m_MtrlScalarData.MAT_3 = *((Matrix*)_Src);
		break;

	}
}







void CGameObject::init()
{
	m_bInitialized = true;

	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->init();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->init();
	}

}

void CGameObject::tick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
		return;


	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
	{
		//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
		cleanup();
		return;
	}
	else if (FLT_MAX_NEG != m_fLifeSpan)
	{
		m_fLifeSpan -= DELTA_TIME;
		if (m_fLifeSpan < 0.f)
		{
			DestroyObject(this);
			return;
		}
	}
		

	//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->finaltick();
	}

	//자녀 포함 모든 컴포넌트가 업데이트 되면 업데이트 상황 초기화
	if (nullptr != m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM])
		static_cast<CTransform*>(m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM])->ClearUpdateState();
}

bool CGameObject::render()
{
	//삭제 대기 상태일 경우 렌더링을 하지 않음.
	if (nullptr == m_RenderCom || true == m_bDestroy)
		return true;

	return m_RenderCom->render();
}

void CGameObject::cleanup()
{
	//본인의 컴포넌트 정리
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->cleanup();
	}

	//자식 컴포넌트들도 모두 cleanup
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->cleanup();
	}
	
	//만약 자신의 부모 오브젝트가 존재하고, 해당 오브젝트는 삭제 대기 상태가 아닐 경우
	//※부모 오브젝트도 삭제 대기 상태일 경우에는 부모 오브젝트도 어차피 이번 프레임 마지막에 삭제가 될 것이므로
	//굳이 자신의 주소를 정리해달라고 요청하지 않아도 된다.
	if (nullptr != m_Parent && false == m_Parent->IsDestroyed())
	{
		m_Parent->RemoveChild(this);
		m_Parent = nullptr;
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();
	assert(nullptr == m_arrCom[ComType]);

	switch ((eCOMPONENT_TYPE)ComType)
	{
	case eCOMPONENT_TYPE::TRANSFORM:
	case eCOMPONENT_TYPE::COLLIDER2D:
	case eCOMPONENT_TYPE::COLLIDER3D:
	case eCOMPONENT_TYPE::ANIMATOR2D:
	case eCOMPONENT_TYPE::ANIMATOR3D:
	case eCOMPONENT_TYPE::LIGHT2D:
	case eCOMPONENT_TYPE::LIGHT3D:
	case eCOMPONENT_TYPE::CAMERA:
		break;

	//Render Components
	case eCOMPONENT_TYPE::MESH_RENDER:
	case eCOMPONENT_TYPE::PARTICLE_SYSTEM:
	case eCOMPONENT_TYPE::TILEMAP:
	case eCOMPONENT_TYPE::LANDSCAPE:
	case eCOMPONENT_TYPE::DECAL:
		//m_RenderCom에 하나 이상의 Render 컴포넌트가 들어가 있을 경우 에러 발생시킴.
		assert(nullptr == m_RenderCom);
		m_RenderCom = static_cast<CRenderComponent*>(_Component);
		break;

	case eCOMPONENT_TYPE::SCRIPT_HOLDER:
		break;
	default:
		break;
	}

	//소유자 주소를 등록.
	_Component->m_pOwner = this;
	m_arrCom[ComType] = _Component;

	//이미 작동중일 경우 바로 init() 호출
	if(m_bInitialized)
		_Component->init();
}

void CGameObject::AddScript(CScript* _Script)
{
	if (nullptr == _Script)
		return;

	CScriptHolder* pScriptHolder = static_cast<CScriptHolder*>(m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]);

	if (nullptr == pScriptHolder)
	{
		pScriptHolder = new CScriptHolder;
		AddComponent(pScriptHolder);
	}
	pScriptHolder->AddScript(_Script);
}

void CGameObject::AddChildObj(CGameObject* _Object)
{
	if (nullptr != (_Object->GetParent()))
		_Object->GetParent()->RemoveChild(_Object);

	_Object->SetParent(this);
	m_vecChild.push_back(_Object);
}

void CGameObject::RemoveChild(CGameObject* _Object)
{
	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();
	while (iter != iterEnd)
	{
		if (_Object == (*iter))
		{
			m_vecChild.erase(iter);
			return;
		}
		++iter;
	}
}


void CGameObject::AddAllHierarchyObjects(int _iLayerIdx, vector<CGameObject*>& _vecObj)
{
	//인덱스가 아직 지정되지 않았을 경우 -> 무조건 인덱스 지정을 한번 해줘야함
	//레이어가 고정되지 않았을 경우 -> 부모 오브젝트의 레이어를 따라감
	//둘중 하나라도 걸리면 일단 옮겨야함
	if (-1 == m_iLayerIdx || false == m_bFixLayer)
	{
		//만약 어딘가의 레이어에 등록되어 있었다면 기존 레이어에서 자신을 삭제해야 한다.
		if (m_iLayerIdx > 0)
		{
			CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx)->RemoveGameObject(this);
		}

		//삭제하고 새로운 레이어에 등록
		m_iLayerIdx = _iLayerIdx;
		_vecObj.push_back(this);
	}
	
	//위 조건의 반대는
	//m_iLayerIdx가 1이 아니고(&&) m_bFixLayer가 true 일때 이므로 이 때는 레이어 이동을 무조건 안하는 경우이다.
	
	//자식들을 대상으로도 재귀적으로 호출
	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->AddAllHierarchyObjects(_iLayerIdx, _vecObj);
	}
}

void CGameObject::SetParentMatrixUpdated()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentMatrixUpdated();
	}
}

bool CGameObject::GetParentWorldMatrix(Matrix& _mat)
{
	//부모 오브젝트가 없거나 트랜스폼이 없을 경우 false 반환
	if (nullptr == m_Parent || nullptr == m_Parent->Transform())
		return false;

	//있을 경우 인자에 그대로 대입, true 반환.
	_mat = m_Parent->Transform()->GetWorldMatWithoutSize();
	return true;
}


void CGameObject::SetChildTransformToUpdate()
{
	if (nullptr != Transform())
		Transform()->SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentMatrixUpdated();
	}
}
