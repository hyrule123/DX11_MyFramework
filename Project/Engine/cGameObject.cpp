#include "pch.h"
#include "cGameObject.h"

#include "components.h"

//레이어 옮길 때 사용
#include "cLevelMgr.h"
#include "cLevel.h"
#include "cLayer.h"

//트랜스폼 상속
#include "cTransform.h"



//생명주기 계산용
#include "cTimeMgr.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

#include "EventDispatcher.h"

#include "CPrefab.h"

//세이브, 로드에 사용
#include "cResMgr.h"

#include "cCom_Renderer_TilemapComplete.h"

namespace JsonKey_cGameObject
{

	//IComponent* m_vecCom[(UINT)eCOMPONENT_TYPE::END];
	STRKEY_DECLARE(m_vecCom);

	//IRenderer* m_RenderCom;

	//Hierarchy
	//cGameObject* m_Parent;
	//있을 경우 Prefab 형태로 따로 저장해서 불러온 뒤 로드해서 주소를 등록
	STRKEY_DECLARE(m_Parent_PREFAB);

	//vector<cGameObject*>    m_vecChild;
	STRKEY_DECLARE(m_vecChild_PREFAB);

	//tMtrlScalarData          m_MtrlScalarData;

	//cLayer Info
	//int                     m_iLayerIdx;
	STRKEY_DECLARE(m_iLayerIdx);

	//레이어 번호를 고정. 부모 레이어를 옮겨도 자신은 옮겨지지 않음.
	//bool                    m_bFixLayer;   
	STRKEY_DECLARE(m_bFixLayer);

	//Birth, Death
	//bool                    m_bDestroy;

	//float                   m_fLifeSpan;
	STRKEY_DECLARE(m_fLifeSpan);

	//초기화 되어 현재 cLevel 안에서 작동중인지 여부를 저장.
	//작동 이후 컴포넌트가 추가될 시 바로 init 호출.
	//bool                m_bInitialized;
}

constexpr int BASIC_VEC_COM_CAPACITY = 20;

cGameObject::cGameObject()
	: m_vecCom((size_t)eCOMPONENT_TYPE::SCRIPTS)
	, m_Parent()
	, m_iLayerIdx(-1)
	, m_fLifeSpan(FLT_MAX_NEGATIVE)
	, m_bDestroy()
	, m_bStart()
	, m_bDisable(false)
	, m_bPrevEnable()
{
	m_vecCom.reserve(BASIC_VEC_COM_CAPACITY);
}

cGameObject::cGameObject(const cGameObject& _other)
	: IEntity(_other)
	, m_vecCom((size_t)eCOMPONENT_TYPE::SCRIPTS)
	, m_iLayerIdx(_other.m_iLayerIdx)
	, m_bDestroy()
	, m_fLifeSpan(FLT_MAX_NEGATIVE)
	, m_bDisable(_other.m_bDisable)
	, m_bPrevEnable(_other.m_bPrevEnable)
{
	m_vecCom.reserve(BASIC_VEC_COM_CAPACITY);

	//1. 컴포넌트 목록 복사
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (_other.m_vecCom[i])
		{
			AddComponent(_other.m_vecCom[i]->Clone());
		}
	}

	//2. 자녀 오브젝트 복사
	size_t size = _other.m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddChildGameObj(_other.m_vecChild[i]->Clone());
	}
}

cGameObject::~cGameObject()
{
	//cLayer에 등록된 cGameObject*의 경우는 cEventMgr을 통해서 제거 할시 알아서 제거됨.
	for (size_t i = 0; i < m_vecCom.size(); ++i)
	{
		SAFE_DELETE(m_vecCom[i]);
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}
}

void cGameObject::SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src)
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


void cGameObject::init()
{
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_vecCom[i])
			m_vecCom[i]->init();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->init();
	}
}

void cGameObject::start()
{
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_vecCom[i])
			m_vecCom[i]->start();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->start();
	}
}

void cGameObject::OnEnable()
{
}

void cGameObject::tick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (m_bDestroy || m_bDisable)
		return;
	else if (false == m_bStart)
	{
		m_bStart = true;
		start();
	}

	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_vecCom[i] && false == m_vecCom[i]->isDisabled())
			m_vecCom[i]->tick();
	}

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->tick();
	}
}

void cGameObject::finaltick()
{
	//자신이 파괴 대기 상태일 경우 자신과 모든 자식들에 대해 tick을 처리하지 않음
	if (true == m_bDestroy)
	{
		//스크립트를 제외한 컴포넌트들에 대해 finaltick()을 호출한다.
		cleanup();
		return;
	}
	else if (m_bDisable)
		return;
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
	for (UINT i = 0; i < m_vecCom.size(); ++i)
	{
		if (nullptr != m_vecCom[i] && false == m_vecCom[i]->isDisabled())
			m_vecCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}


	//자녀 포함 모든 컴포넌트가 업데이트 되면 트랜스폼의 업데이트 상황 초기화
	Transform().ClearUpdateState();
}

bool cGameObject::render()
{
	//삭제 대기 상태일 경우 렌더링을 하지 않음.
	if (nullptr == Renderer() || true == m_bDestroy)
		return true;

	bool rendered = Renderer()->render();

	return rendered;
}

void cGameObject::cleanup()
{
	//본인의 컴포넌트 정리
	for (UINT i = 0; i < (UINT)eCOMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_vecCom[i])
			m_vecCom[i]->cleanup();
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


bool cGameObject::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IEntity::SaveJson(_pJson))
		return false;

	if (GetKey().empty())
	{
		ERROR_MESSAGE("You must Set name to save prefab!!");
		return false;
	}

	Json::Value& jVal = *_pJson;

	string strKeyarrCom = JsonKey_cGameObject::m_vecCom;
	jVal[strKeyarrCom] = Json::Value(Json::ValueType::arrayValue);
	for (int i = 0; i < (int)eCOMPONENT_TYPE::END; ++i)
	{
		Json::Value& arrCom = jVal[strKeyarrCom];
		if (nullptr == m_vecCom[i])
		{
			//빈 데이터를 삽입
			arrCom.append(Json::Value(Json::ValueType::nullValue));
		}
		else
		{
			Json::Value compVal;
			if (false == m_vecCom[i]->SaveJson(&compVal))
				return false;

			arrCom.append(compVal);
		}
	}

	{
		string strKey = string(JsonKey_cGameObject::m_vecChild_PREFAB);
		Json::Value& vecChild = jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		
		for (size_t i = 0u; i < m_vecChild.size(); ++i)
		{
			//자식 오브젝트의 이름을 받아온다.
			string childKey(m_vecChild[i]->GetKey());

			//이름이 없을 경우 직접 이름을 생성
			if (childKey.empty())
			{
				childKey = GetKey();

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
		string strKey = string(JsonKey_cGameObject::m_iLayerIdx);
		jVal[strKey] = m_iLayerIdx;
	}

	{
		string strKey = string(JsonKey_cGameObject::m_fLifeSpan);
		jVal[strKey] = Pack_float_int(m_fLifeSpan).i;
	}


	return true;
}

bool cGameObject::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == IEntity::LoadJson(_pJson))
		return false;

	Json::Value& jVal = *_pJson;



	string strKeyarrCom = string(JsonKey_cGameObject::m_vecCom);
	if (jVal.isMember(strKeyarrCom))
	{
		//TODO: 여기 cUserClassMgr과 연동할 것

		Json::Value& arrCom = jVal[strKeyarrCom];
		//int arrComSize = (int)arrCom.size();
		//for (int i = 0; i < arrComSize; ++i)
		//{
		//	if (arrCom[i].empty())
		//		continue;

		//	Json::Value& jsonComponent = arrCom[i];

		//	IComponent* pCom = nullptr;
		//	switch ((eCOMPONENT_TYPE)i)
		//	{
		//		
		//	case eCOMPONENT_TYPE::COLLIDER2D:
		//	{
		//		eCOLLIDER_TYPE_2D ColType = (eCOLLIDER_TYPE_2D)jsonComponent[string(RES_INFO::PREFAB::COMPONENT::COLLIDER2D::JSON_KEY::m_eColType)].asInt();

		//		switch (ColType)
		//		{
		//		case eCOLLIDER_TYPE_2D::RECT:
		//			pCom = new cCom_Coll2D_Rect;
		//			break;
		//		case eCOLLIDER_TYPE_2D::CIRCLE:
		//			pCom = new cCom_Coll2D_Circle;
		//			break;
		//		case eCOLLIDER_TYPE_2D::OBB:
		//			pCom = new cCom_Coll2D_OBB;
		//			break;
		//		case eCOLLIDER_TYPE_2D::POINT:
		//			pCom = new cCom_Coll2D_Point;
		//			break;
		//		default:
		//			break;
		//		}

		//		break;
		//	}
		//		
		//	case eCOMPONENT_TYPE::COLLIDER3D:
		//		break;
		//	case eCOMPONENT_TYPE::ANIMATOR2D:
		//		pCom = new cCom_Animator2D;
		//		break;
		//	case eCOMPONENT_TYPE::ANIMATOR3D:
		//		break;
		//	case eCOMPONENT_TYPE::LIGHT2D:
		//		break;
		//	case eCOMPONENT_TYPE::LIGHT3D:
		//		break;
		//	case eCOMPONENT_TYPE::CAMERA:
		//		break;

		//	case eCOMPONENT_TYPE::RENDERER:
		//	case eCOMPONENT_TYPE::MESH_RENDER:
		//		pCom = new cCom_Renderer_Basic;
		//		break;
		//	case eCOMPONENT_TYPE::PARTICLE_SYSTEM:
		//		break;
		//	case eCOMPONENT_TYPE::TILEMAP:
		//	{
		//		const char* strKey = RES_INFO::PREFAB::COMPONENT::RENDER_COMP::TILEMAP::m_TilemapType;
		//		if (jsonComponent.isMember(strKey))
		//		{
		//			eTILEMAP_TYPE type = (eTILEMAP_TYPE)jsonComponent[strKey].asInt();

		//			switch (type)
		//			{
		//			case eTILEMAP_TYPE::ATLAS:
		//				pCom = new cRenderer_Tilemap;
		//				break;
		//			case eTILEMAP_TYPE::COMPLETE:
		//				pCom = new cCom_Renderer_TilemapComplete;
		//				break;
		//			default:
		//				ERROR_MESSAGE("ITilemapBase Load Error");
		//				break;
		//			}
		//		}

		//	}



		//		break;
		//	case eCOMPONENT_TYPE::LANDSCAPE:
		//		break;
		//	case eCOMPONENT_TYPE::DECAL:
		//		break;
		//	case eCOMPONENT_TYPE::SCRIPT_HOLDER:
		//		pCom = new cScriptHolder;
		//		break;
		//	case eCOMPONENT_TYPE::END:
		//		break;
		//	default:
		//		break;
		//	}

		//	if (pCom)
		//	{
		//		//LoadJson -> Prefab 만드는 데 사용
		//		//init이 먼저 호출되지 않음
		//		//->IComponent를 먼저 추가해도 문제 없음
		//		AddComponent(pCom);

		//		if (false == pCom->LoadJson(&jsonComponent))
		//		{
		//			return false;
		//		}
		//	}
		//}
	}
	else return false;

	//계층 프리팹 로드
	{
		string strKey = string(JsonKey_cGameObject::m_vecChild_PREFAB);
		if (false == jVal.isMember(strKey))
			return false;

		const Json::Value& arrChild = jVal[strKey];
		for (Json::ValueConstIterator iter = arrChild.begin(); iter != arrChild.end(); ++iter)
		{
			Ptr<CPrefab> pPrefab = cResMgr::GetInst()->Load<CPrefab>(iter->asString());
			if (nullptr == pPrefab)
			{
				ERROR_MESSAGE("Child Prefab load failed.");
				DEBUG_BREAK;
				return false;
			}
			cGameObject* pObj = pPrefab->Instantiate();
			
			AddChildGameObj(pObj);
		}
	}

	{
		string strKey = string(JsonKey_cGameObject::m_iLayerIdx);
		if (jVal.isMember(strKey))
		{
			m_iLayerIdx = jVal[strKey].asInt();
		}
		else return false;
	}

	{
		string strKey = string(JsonKey_cGameObject::m_fLifeSpan);
		if (jVal.isMember(strKey))
		{
			m_fLifeSpan = Pack_float_int(jVal[strKey].asInt()).f;
		}
		else return false;
	}

	return true;
}


void cGameObject::AddComponent(IComponent* _Component)
{
	UINT ComType = (UINT)_Component->GetType();

	//동일 컴포넌트 중복 등록시 에러 발생
	assert(nullptr == m_vecCom[ComType]);

	_Component->SetOwner(this);
	if ((UINT)eCOMPONENT_TYPE::SCRIPTS != ComType)
	{
		//소유자 주소를 등록.
		m_vecCom[ComType] = _Component;
	}

	//스크립트는 push_back 형태로 뒤에 하나씩 붙임
	else
	{
		m_vecCom.push_back(_Component);
	}
}

void cGameObject::RemoveComponent(eCOMPONENT_TYPE _eComType)
{
	if (nullptr == m_vecCom[(UINT)_eComType])
		return;

	//Disable되어있지 않을 경우 제거 X(오류 발생 가능성)
	else if (false == m_vecCom[(UINT)_eComType]->isDisabled())
		return;

	SAFE_DELETE(m_vecCom[(UINT)_eComType]);
}

const std::span<IComponent*> cGameObject::GetScripts()
{
	
}

//void cGameObject::AddScript(IScript* _Script)
//{
//	if (nullptr == _Script)
//		return;
//
//	cScriptHolder* pScriptHolder = static_cast<cScriptHolder*>(m_vecCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]);
//
//	if (nullptr == pScriptHolder)
//	{
//		pScriptHolder = new cScriptHolder;
//		AddComponent(pScriptHolder);
//	}
//	pScriptHolder->AddScript(_Script);
//}

void cGameObject::AddChildGameObj(cGameObject* _Object)
{
	//nullptr이나 자기 자신을 인자로 호출했을 경우 오류 발생			
	assert(_Object && this != _Object);
	if (nullptr != (_Object->GetParent()))
		_Object->GetParent()->RemoveChild(_Object);

	_Object->SetParent(this);
	m_vecChild.push_back(_Object);
}

void cGameObject::RemoveChild(cGameObject* _Object)
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


void cGameObject::SetParentMatrixUpdated()
{
	Transform().SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentMatrixUpdated();
	}
}

bool cGameObject::GetParentWorldMatrix(Matrix& _mat)
{
	//부모 오브젝트가 없거나 트랜스폼이 없을 경우 false 반환
	if (nullptr == m_Parent)
		return false;

	//있을 경우 인자에 그대로 대입, true 반환.
	_mat = m_Parent->Transform().GetWorldMatWithoutSize();
	return true;
}


void cGameObject::SetChildTransformToUpdate()
{
	Transform().SetParentUpdate();

	size_t size = m_vecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecChild[i]->SetParentMatrixUpdated();
	}
}
