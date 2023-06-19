#include "pch.h"
#include "CGameObject.h"

#include "components.h"

//레이어 옮길 때 사용
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//트랜스폼 상속
#include "CTransform.h"
#include "CTilemap_SC.h"


//생명주기 계산용
#include "CTimeMgr.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

#include "EventDispatcher.h"

#include "CPrefab.h"

//세이브, 로드에 사용
#include "CResMgr.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom()
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_fLifeSpan(FLT_MAX_NEGATIVE)
	, m_bDestroy()
	, m_bStart()
	, m_bEnable(true)
	, m_bPrevEnable()
{
	AddComponent(new CTransform);
}

CGameObject::CGameObject(const CGameObject& _other)
	: CEntity(_other)
	, m_arrCom{}
	, m_iLayerIdx(_other.m_iLayerIdx)
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEGATIVE)
	, m_bEnable(_other.m_bEnable)
	, m_bPrevEnable(_other.m_bPrevEnable)
{
	//1. 컴포넌트 목록 복사
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (_other.m_arrCom[i])
		{
			AddComponent(_other.m_arrCom[i]->Clone());
		}
	}

	//2. 자녀 오브젝트 복사
	size_t size = _other.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddChildGameObj(_other.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	//Layer에 등록된 CGameObject*의 경우는 EventMgr을 통해서 제거 할시 알아서 제거됨.
	Safe_Del_Array(m_arrCom);

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
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

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::OnEnable()
{
}

void CGameObject::tick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
		return;
	else if (false == m_bStart)
	{
		m_bStart = true;
		start();
	}
		

	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && false == m_arrCom[i]->isDisabled())
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
	else if (FLT_MAX_NEGATIVE != m_fLifeSpan)
	{
		m_fLifeSpan -= DELTA_TIME;
		if (m_fLifeSpan < 0.f)
		{
			EventDispatcher::DestroyGameObj(this);
			return;
		}
	}
		
	

	//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER; ++i)
	{
		if (nullptr != m_arrCom[i] && false == m_arrCom[i]->isDisabled())
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


bool CGameObject::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CEntity::SaveJson(_pJson))
		return false;

	if (GetName().empty())
	{
		ERROR_MESSAGE("You must Set name to save prefab!!");
		return false;
	}

	Json::Value& jVal = *_pJson;

	string strKeyarrCom = string(RES_INFO::PREFAB::JSON_KEY::m_arrCom);
	jVal[strKeyarrCom] = Json::Value(Json::ValueType::arrayValue);
	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		Json::Value& arrCom = jVal[strKeyarrCom];
		if (nullptr == m_arrCom[i])
		{
			//빈 데이터를 삽입
			arrCom.append(Json::Value(Json::ValueType::nullValue));
		}
		else
		{
			Json::Value compVal;
			if (false == m_arrCom[i]->SaveJson(&compVal))
				return false;

			arrCom.append(compVal);
		}
	}

	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_vecChild_PREFAB);
		Json::Value& vecChild = jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		
		for (size_t i = 0u; i < m_vecChild.size(); ++i)
		{
			//자식 오브젝트의 이름을 받아온다.
			string childKey = m_vecChild[i]->GetName();

			if (childKey.empty())
			{
				childKey = GetName();

				//혹시나 json 확장자 있을경우 확장자 제거
				size_t pos = childKey.find(RES_INFO::PREFAB::Ext);
				if (string::npos != pos)
					childKey.erase(pos, string::npos);

				childKey += string("_Child_") + std::to_string(i);
			}
			else
			{
				//혹시나 json 확장자 있을경우 확장자 제거
				size_t pos = childKey.find(RES_INFO::PREFAB::Ext);
				if (string::npos != pos)
					childKey.erase(pos, string::npos);
			}
			childKey += RES_INFO::PREFAB::Ext;

			CPrefab* Prefab = new CPrefab;
			Prefab->SetKey(childKey);

			//Save 모드로 프리팹을 생성한 뒤 저장
			Prefab->RegisterPrefab(m_vecChild[i], true);

			bool Suc = Prefab->Save(childKey);
			delete Prefab;

			if (false == Suc)
				return false;

			//자식 오브젝트의 키값만 저장(프리팹의 키값으로 사용됨)
			vecChild.append(childKey);
		}
	}

	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_iLayerIdx);
		jVal[strKey] = m_iLayerIdx;
	}

	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_fLifeSpan);
		jVal[strKey] = Pack_float_int(m_fLifeSpan).i;
	}


	return true;
}

bool CGameObject::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CEntity::LoadJson(_pJson))
		return false;

	Json::Value& jVal = *_pJson;

	string strKeyarrCom = string(RES_INFO::PREFAB::JSON_KEY::m_arrCom);
	if (jVal.isMember(strKeyarrCom))
	{
		Json::Value& arrCom = jVal[strKeyarrCom];
		int arrComSize = (int)arrCom.size();
		for (int i = 0; i < arrComSize; ++i)
		{
			if (arrCom[i].empty())
				continue;

			Json::Value& jsonComponent = arrCom[i];

			CComponent* pCom = nullptr;
			switch ((eCOMPONENT_TYPE)i)
			{
			case eCOMPONENT_TYPE::TRANSFORM:
			{
				Transform()->LoadJson(&jsonComponent);
				break;
			}
				
			case eCOMPONENT_TYPE::COLLIDER2D:
			{
				eCOLLIDER_TYPE_2D ColType = (eCOLLIDER_TYPE_2D)jsonComponent[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType)].asInt();

				switch (ColType)
				{
				case eCOLLIDER_TYPE_2D::RECT:
					pCom = new CCollider2D_Rect;
					break;
				case eCOLLIDER_TYPE_2D::CIRCLE:
					pCom = new CCollider2D_Circle;
					break;
				case eCOLLIDER_TYPE_2D::OBB:
					pCom = new CCollider2D_OBB;
					break;
				case eCOLLIDER_TYPE_2D::POINT:
					pCom = new CCollider2D_Point;
					break;
				default:
					break;
				}

				break;
			}
				
			case eCOMPONENT_TYPE::COLLIDER3D:
				break;
			case eCOMPONENT_TYPE::ANIMATOR2D:
				pCom = new CAnimator2D;
				break;
			case eCOMPONENT_TYPE::ANIMATOR3D:
				break;
			case eCOMPONENT_TYPE::LIGHT2D:
				break;
			case eCOMPONENT_TYPE::LIGHT3D:
				break;
			case eCOMPONENT_TYPE::CAMERA:
				break;
			case eCOMPONENT_TYPE::MESH_RENDER:
				pCom = new CMeshRender;
				break;
			case eCOMPONENT_TYPE::PARTICLE_SYSTEM:
				break;
			case eCOMPONENT_TYPE::TILEMAP:
				pCom = new CTilemap_SC;
				break;
			case eCOMPONENT_TYPE::LANDSCAPE:
				break;
			case eCOMPONENT_TYPE::DECAL:
				break;
			case eCOMPONENT_TYPE::SCRIPT_HOLDER:
				pCom = new CScriptHolder;
				break;
			case eCOMPONENT_TYPE::END:
				break;
			default:
				break;
			}

			if (pCom)
			{
				//LoadJson -> Prefab 만드는 데 사용
				//init이 먼저 호출되지 않음
				//->Component를 먼저 추가해도 문제 없음
				AddComponent(pCom);

				if (false == pCom->LoadJson(&jsonComponent))
				{
					return false;
				}
			}
		}
	}
	else return false;

	//계층 프리팹 로드
	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_vecChild_PREFAB);
		if (false == jVal.isMember(strKey))
			return false;

		const Json::Value& arrChild = jVal[strKey];
		for (Json::ValueConstIterator iter = arrChild.begin(); iter != arrChild.end(); ++iter)
		{
			Ptr<CPrefab> pPrefab = CResMgr::GetInst()->Load<CPrefab>(iter->asString());
			if (nullptr == pPrefab)
			{
				ERROR_MESSAGE("Child Prefab load failed.");
				DEBUG_BREAK;
				return false;
			}
			CGameObject* pObj = pPrefab->Instantiate();
			
			AddChildGameObj(pObj);
		}
	}

	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_iLayerIdx);
		if (jVal.isMember(strKey))
		{
			m_iLayerIdx = jVal[strKey].asInt();
		}
		else return false;
	}

	{
		string strKey = string(RES_INFO::PREFAB::JSON_KEY::m_fLifeSpan);
		if (jVal.isMember(strKey))
		{
			m_fLifeSpan = Pack_float_int(jVal[strKey].asInt()).f;
		}
		else return false;
	}

	return true;
}




void CGameObject::AddComponent(CComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();

	//동일 컴포넌트 중복 등록시 에러 발생
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
	_Component->SetOwner(this);
	m_arrCom[ComType] = _Component;
}

void CGameObject::RemoveComponent(eCOMPONENT_TYPE _eComType)
{
	if (nullptr == m_arrCom[(UINT)_eComType])
		return;

	//Disable되어있지 않을 경우 제거 X(오류 발생 가능성)
	else if (false == m_arrCom[(UINT)_eComType]->isDisabled())
		return;

	SAFE_DELETE(m_arrCom[(UINT)_eComType]);
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

void CGameObject::AddChildGameObj(CGameObject* _Object)
{
	assert(_Object);
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
			//부모 오브젝트 주소 제거
			(*iter)->SetParent(nullptr);
			m_vecChild.erase(iter);
			return;
		}
		++iter;
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
