#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"

#include "CRenderMgr.h"
#include "CMeshRender.h"

//자신이 업데이트 되면 충돌체도 업데이트 해준다.
#include "CCollider2D.h"
#include "CCollider3D.h"

#include "strKey_Default.h"
#include "jsoncpp.h"



CTransform::CTransform()
	: CComponent(eCOMPONENT_TYPE::TRANSFORM)
	, m_v3Size(100.f, 100.f, 100.f)
	, m_v3RelativeScale(1.f, 1.f, 1.f)
	, m_bIsDefaultScale(true)
	, m_bInheritScale(true)
	, m_bInheritRot(true)
	, m_bSizeUpdated(true)
	, m_bLockRotation()
	, m_bNeedMyUpdate(true)
	, m_bNeedParentUpdate(true)
	, m_fLongestDiagonalLen()
	, m_fYsortDepthPreset(DEPTH_PRESET_MAX)
	//Matrix와 Vector 변수는 자체 생성자를 통해 초기화 됨.
{
}

CTransform::~CTransform()
{
}



void CTransform::finaltick()
{
	//bool 값들은 tick()에서 false로 초기화 된다.
	//여기선 고유 크기(Size)를 반영하지 않은 월드행렬을 만든다.
	//게임오브젝트 상속 관계에서 고유 크기까지 상속을 받게 되면 기하급수적으로 크기가 커짐 
	if (true == m_bNeedMyUpdate)
	{
		//자신의 트랜스폼 업데이트를 진행할 경우 - 두개 다 업데이트 해줘야함.
		UpdateMyTransform();
	}
	//부모 트랜스폼'만' 갱신되었을 경우 : 자신은 갱신할 필요 없음.
	if (true == m_bNeedParentUpdate)
	{
		UpdateParentMatrix();
	}


	bool Updated = false;
	//둘중에 하나라도 업데이트 되었을 경우 월드행렬을 새로 계산한다.
	if (m_bNeedMyUpdate || m_bNeedParentUpdate || m_bSizeUpdated)
	{
		Updated = true;

		//부모 행렬이 있을 경우 부모행렬을 곱해줌.
		if (GetOwner()->GetParent())
			m_matWorldWithoutSize = m_matRelative * m_matParent;
		else
			m_matWorldWithoutSize = m_matRelative;

		//자신의 사이즈가 반영된 최종 월드행렬을 계산
		m_matWorld = Matrix::CreateScale(m_v3Size) * m_matWorldWithoutSize;

		//행렬의 1 ~ 3 부분을 전부 제곱한 뒤, 제곱근을 씌운다. 그러면 가장 긴 대각선의 길이가 나온다.
		//sqrt(11제곱 + 12제곱 + 13제곱 + 21제곱 + 22제곱 + 23제곱 + 31제곱 + 32제곱 + 33제곱)
		m_fLongestDiagonalLen = sqrtf(m_matWorld.Axis((int)eAXIS3D::X).LengthSquared() + m_matWorld.Axis((int)eAXIS3D::Y).LengthSquared() + m_matWorld.Axis((int)eAXIS3D::Z).LengthSquared());
	}

	//깊이 프리셋 최대값보다 조금이라도 작을 시 Y Sorting을 적용
	if (m_fYsortDepthPreset < DEPTH_PRESET_MAX)
	{
		Updated = true;
		//프리셋값에 y값을 더할경우 정렬을 하지 않고도 Y Sorting이 가능
		//Y값이 높을수록 위에 있으므로(2D)
		//Y값은 맵사이즈만큼 줄여서 사용
		m_matWorld.m[3][2] = m_fYsortDepthPreset + (m_matWorld.m[3][1] / g_GlobalVal.fMapSizeY);
	}

	if (Updated)
		GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MAT_WORLD, &m_matWorld);
}

bool CTransform::SaveJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;

	//Transform 항목을 하나 만들어서 그 안에 저장
	Json::Value& jVal = (*_pJson);

	//상위 컴포넌트 항목들도 마찬가지로 내부에 저장한다.
	if (false == CComponent::SaveJson(&jVal))
		return false;

	
	{//사이즈 X, Y, Z 순서로 저장
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3Size.x).i);
		jVal[strKey].append(Pack_float_int(m_v3Size.y).i);
		jVal[strKey].append(Pack_float_int(m_v3Size.z).i);
	}

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3RelativeScale.x).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativeScale.y).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativeScale.z).i);
	}

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3RelativePos.x).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativePos.y).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativePos.z).i);
	}

	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
		jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		jVal[strKey].append(Pack_float_int(m_v3RelativeRot.x).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativeRot.y).i);
		jVal[strKey].append(Pack_float_int(m_v3RelativeRot.z).i);
	}

	{
		jVal[string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot)] = m_bInheritRot;
		jVal[string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale)] = m_bInheritScale;
		jVal[string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot)] = m_bLockRotation;
	}

	return true;
}

bool CTransform::LoadJson(Json::Value* _pJson)
{
	if (nullptr == _pJson)
		return false;
	else if (false == CComponent::LoadJson(_pJson))
		return false;

	Json::Value& jVal = (*_pJson);

	auto ReadVec3FromJson = [](const Json::Value& _jVal, const string& _strKey, Vec3& _outVal)->bool
	{
		if (_jVal.isMember(_strKey))
		{
			if (_jVal[_strKey].size() != 3)
				return false;

			_outVal.x = Pack_float_int(_jVal[_strKey][0].asInt()).f;
			_outVal.y = Pack_float_int(_jVal[_strKey][1].asInt()).f;
			_outVal.z = Pack_float_int(_jVal[_strKey][2].asInt()).f;

			return true;
		}
		else
			return false;
	};



	{//RelativeSize
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
		Vec3 size;
		if (ReadVec3FromJson(jVal, strKey, size))
		{
			SetSize(size);
		}
		else
			return false;
	}


	{//RelativeScale
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
		Vec3 scale;
		if (ReadVec3FromJson(jVal, strKey, scale))
		{
			SetRelativeScale(scale);
		}
		else
			return false;
	}

	{//RelativePos
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
		Vec3 pos;
		if (ReadVec3FromJson(jVal, strKey, pos))
		{
			SetRelativePos(pos);
		}
		else
			return false;
	}

	{//RelativeRot
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
		Vec3 rot;
		if (ReadVec3FromJson(jVal, strKey, rot))
		{
			SetRelativeRot(rot);
		}
		else
			return false;
	}


	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot);
		if (jVal.isMember(strKey))
			m_bInheritRot = jVal[strKey].asBool();
		else return false;

		strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale);
		if (jVal.isMember(strKey))
			m_bInheritScale = jVal[strKey].asBool();
		else return false;

		strKey = string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot);
		if (jVal.isMember(strKey))
		{
			m_bLockRotation = jVal[strKey].asBool();
		}
		else return false;
	}


	return true;
}



void CTransform::UpdateMyTransform()
{
	m_matRelative = Matrix::Identity;

	//1. 크기행렬
	if (false == m_bIsDefaultScale)
	{
		//크기행렬(CreateScale을 해주면 자동으로 동차좌표를 추가해서 행렬에 삽입해 준다.
		m_matRelative *= Matrix::CreateScale(m_v3RelativeScale);
	}

	//2. 회전행렬
	Matrix matRot = MATRIX::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(m_v3RelativeRot.y, m_v3RelativeRot.x, m_v3RelativeRot.z));
	//회전행렬으로부터 직관적 방향을 계산한다.
	m_vRelativeDir[(UINT)eDIR_TYPE::FRONT] = matRot.Forward();
	m_vRelativeDir[(UINT)eDIR_TYPE::RIGHT] = matRot.Right();
	m_vRelativeDir[(UINT)eDIR_TYPE::UP] = matRot.Up();
	//방향은 쿼터니언을 사용해서 계산.
	
	//회전 잠금 상태가 아닐 경우에만 회전행렬을 곱해준다.
	if (false == m_bLockRotation)
		m_matRelative *= matRot;

	//3. 이동행렬
	m_matRelative *= Matrix::CreateTranslation(m_v3RelativePos);
}

void CTransform::UpdateParentMatrix()
{
	m_matParent = Matrix::Identity;

	//부모 오브젝트가 있을 경우 부모의 월드행렬을 받아온다. 
	//성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
	bool bWorldDirInherit = false;
	if (true == GetOwner()->GetParentWorldMatrix(m_matParent))
	{
		if (true == m_bInheritRot)
		{
			//회전 상속 + 크기 미상속 -> 크기정보 제거
			if (false == m_bInheritScale)
			{
				//정규화해서 크기정보를 제거
				m_matParent.Right(m_matParent.Right().Normalize());
				m_matParent.Up(m_matParent.Up().Normalize());
				m_matParent.Forward(m_matParent.Forward().Normalize());
			}
			//else: 둘 다 상속 받는 경우에는 작업할 것이 없음. 그냥 빠져나가면 됨

			bWorldDirInherit = true;	//이때만 월드방향을 상속받아주면 된다.
		}
		else
		{
			//회전 미상속 + 크기 상속 -> 회전정보 제거
			if (true == m_bInheritScale)
			{
				//회전정보만 상속받는 경우: 크기정보만 추출
				Vec3 Scale(m_matParent.Right().Length(), m_matParent.Up().Length(), m_matParent.Forward().Length());
				//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록
				memset(m_matParent.m, 0, 48);
				m_matParent._11 = Scale.x;
				m_matParent._22 = Scale.y;
				m_matParent._33 = Scale.z;
			}
			//회전 미상속 + 크기 미상속 -> 전부 밀고 단위행렬로
			else
			{
				memset(m_matParent.m, 0, 48);
				m_matParent._11 = 1.f;
				m_matParent._22 = 1.f;
				m_matParent._33 = 1.f;
			}
		}
	}
}

