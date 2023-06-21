#include "pch.h"
#include "CAnim2DAtlas.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CStructBuffer.h"

#include "jsoncpp.h"

CAnim2DAtlas::CAnim2DAtlas()
	: CRes(eRES_TYPE::ANIM2D_ATLAS)
	, m_uColTotal()
	, m_uRowTotal()
	, m_bRegularFrameSize()
{
}

CAnim2DAtlas::~CAnim2DAtlas()
{
}

//bool CAnim2DAtlas::Save(const std::filesystem::path& _fileName)
//{
//	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
//	std::filesystem::path FilePath = RELATIVE_PATH::RES_ARR[(int)GetResType()];
//	FilePath /= _fileName;
//	
//	std::ofstream outFile(FilePath);
//	if (outFile.is_open())
//	{
//		Json::Value SaveVal;
//
//		bool Suc = SaveJson(&SaveVal);
//		if (true == Suc)
//		{
//			outFile << SaveVal;
//		}
//		outFile.close();
//
//		return Suc;
//	}
//
//	return false;
//}
//
//bool CAnim2DAtlas::Load(const std::filesystem::path& _fileName)
//{
//
//}



//bool CAnim2DAtlas::Save(const std::filesystem::path& _fileName)
//{
//	//ResType을 인덱스로 써서 상대경로를 받아올 수 있다.
//	std::filesystem::path FilePath = GETRESPATH;
//	FilePath /= _fileName;
//
//	std::ofstream outFile(FilePath);
//	if (outFile.is_open())
//	{
//		Json::Value SaveVal;
//
//		bool Suc = SaveJson(&SaveVal);
//		if (true == Suc)
//		{
//			Json::StreamWriterBuilder builder;
//			builder["indentation"] = ""; //The JSON document is written in a single line
//			std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
//			writer->write(SaveVal, &outFile);
//		}
//		outFile.close();
//
//		return Suc;
//	}
//
//	return false;
//}

bool CAnim2DAtlas::Save(const std::filesystem::path& _fileName)
{
	//확장자에 .json을 더해서 저장.(저장 및 불러오기 할때만 확장자를 더하고 뺌.)
	//로드 이후 키값에서는 .json 확장자를 제거
	std::filesystem::path AddExt = _fileName;
	AddExt += RES_INFO::ANIM2D::Ext;

	return CRes::Save(AddExt);
}

bool CAnim2DAtlas::Load(const std::filesystem::path& _fileName)
{
	std::filesystem::path AddExt = _fileName;
	SetKey(AddExt.string());

	AddExt += RES_INFO::ANIM2D::Ext;

	//확장자를 json으로 바꿔서 load 한뒤
	return CRes::Load(AddExt);
}

//TODO : 여기 작성
bool CAnim2DAtlas::SaveJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == CRes::SaveJson(_jVal))
		return false;

	Json::Value& jVal = *_jVal;

	//텍스처가 없으면 false를 리턴
	if (nullptr == m_AtlasTex)
		return false;

	jVal[RES_INFO::ANIM2D::JSON_KEY::strKeyAtlasTex] = m_AtlasTex->GetKey();
	jVal[RES_INFO::ANIM2D::JSON_KEY::bRegularFrameSize] = m_bRegularFrameSize;
	


	jVal[RES_INFO::ANIM2D::JSON_KEY::vecFrameUV] = Json::Value(Json::ValueType::objectValue);
	{
		Json::Value& FrameUV = jVal[RES_INFO::ANIM2D::JSON_KEY::vecFrameUV];
		FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVLeftTop] = Json::Value(Json::arrayValue);
		FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVSlice] = Json::Value(Json::arrayValue);
		FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_Offset] = Json::Value(Json::arrayValue);
		for (size_t i = 0; i < m_vecFrameUV.size(); ++i)
		{
			const tAnimFrameUV& Frame = m_vecFrameUV[i];

			//vec2는 float 2개(4byte * 2) -> union으로 묶은뒤 UINT64(8byte)에 묶어서 저장한다.
			FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVLeftTop].append(Pack_v2_i64(Frame.v2_UVLeftTop).i64);

			FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVSlice].append(Pack_v2_i64(Frame.v2_UVSlice).i64);

			FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_Offset].append(Pack_v2_i64(Frame.v2_Offset).i64);
		}
	}

	{
		jVal[RES_INFO::ANIM2D::JSON_KEY::mapAnim] = Json::Value(Json::objectValue);
		Json::Value& mapAnim = jVal[RES_INFO::ANIM2D::JSON_KEY::mapAnim];

		for (const auto& iter : m_mapAnim)
		{
			mapAnim[iter.first] = Json::Value(Json::objectValue);
			Json::Value& anim2dVal = mapAnim[iter.first];

			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::eAnimType] = (int)iter.second.eAnimType;
			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uColTotal] = iter.second.uColTotal;
			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uRowTotal] = iter.second.uRowTotal;
			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uNumFrame] = iter.second.uNumFrame;

			//float은 int 값으로 변환해서 저장한다.
			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::fFullPlayTime] = Pack_float_int(iter.second.fFullPlayTime).i;
			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::vPivot] = Pack_v2_i64(iter.second.vPivot).i64;

			anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::vecFrame] = Json::Value(Json::ValueType::arrayValue);
			Json::Value& vecFrame = anim2dVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::vecFrame];

			for (size_t i = 0; i < iter.second.vecFrame.size(); ++i)
			{
				vecFrame.append(iter.second.vecFrame[i]);
			}
		}
	}

	return true;
}

bool CAnim2DAtlas::LoadJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == CRes::LoadJson(_jVal))
		return false;

	const Json::Value& jVal = *_jVal;

	if (jVal.isMember(RES_INFO::ANIM2D::JSON_KEY::strKeyAtlasTex))
	{
		const string& strKey = jVal[RES_INFO::ANIM2D::JSON_KEY::strKeyAtlasTex].asString();
		Ptr<CTexture> tex = CResMgr::GetInst()->Load<CTexture>(strKey);
		if (nullptr == tex)
		{
			ERROR_MESSAGE(string("Atlas Texture " + strKey + "Load Failed!!").c_str());
			DEBUG_BREAK;
			return false;
		}
		SetAtlasTexture(tex);
	}

	if (jVal.isMember(RES_INFO::ANIM2D::JSON_KEY::bRegularFrameSize))
	{
		m_bRegularFrameSize = jVal[RES_INFO::ANIM2D::JSON_KEY::bRegularFrameSize].asBool();
	}

	if (jVal.isMember(RES_INFO::ANIM2D::JSON_KEY::vecFrameUV))
	{
		const Json::Value& FrameUV = jVal[RES_INFO::ANIM2D::JSON_KEY::vecFrameUV];
		if (
			FrameUV.isMember(RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVLeftTop)
			&&
			FrameUV.isMember(RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVSlice)
			&&
			FrameUV.isMember(RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_Offset)
			)
		{
			const Json::Value& v2_UVLeftTop = FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVLeftTop];
			const Json::Value& v2_UVSlice = FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_UVSlice];
			const Json::Value& v2_Offset = FrameUV[RES_INFO::ANIM2D::JSON_KEY::AnimFrameUV::v2_Offset];
			if (v2_UVLeftTop.size()
				!=
				v2_UVSlice.size()
				&&
				v2_UVSlice.size()
				!=
				v2_Offset.size()
				)
			{
				string ErrorMessage("AnimFrameUV Data in\n");
				ErrorMessage += GetKey();
				ErrorMessage += "is corrupted!!";
				MessageBoxA(nullptr, ErrorMessage.c_str(), nullptr, MB_OK);
				return false;
			}
			Json::ValueConstIterator LTIter = v2_UVLeftTop.begin();
			Json::ValueConstIterator SliceIter = v2_UVSlice.begin();
			Json::ValueConstIterator OffsetIter = v2_Offset.begin();

			size_t size = v2_UVLeftTop.size();
			for (size_t i = 0; i < size; ++i)
			{
				tAnimFrameUV FrameUV = {};
				FrameUV.v2_UVLeftTop = Pack_v2_i64(LTIter->asInt64()).v2;
				FrameUV.v2_UVSlice = Pack_v2_i64(SliceIter->asInt64()).v2;
				FrameUV.v2_Offset = Pack_v2_i64(OffsetIter->asInt64()).v2;

				m_vecFrameUV.push_back(FrameUV);

				++LTIter;
				++SliceIter;
				++OffsetIter;
			}
		}
	}

	{
		if (jVal.isMember(RES_INFO::ANIM2D::JSON_KEY::mapAnim))
		{
			const Json::Value& mapAnim = jVal[RES_INFO::ANIM2D::JSON_KEY::mapAnim];

			const Json::ValueConstIterator& mapIterEnd = mapAnim.end();
			for (Json::ValueConstIterator mapIter = mapAnim.begin(); mapIter != mapIterEnd; ++mapIter)
			{
				tAnim2D anim = {};
				anim.strKeyAnim2D = mapIter.key().asString();
				
				const Json::Value& animVal = *mapIter;
				anim.eAnimType = (eANIM_TYPE)animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::eAnimType].asInt();
				anim.uColTotal = animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uColTotal].asUInt();
				anim.uRowTotal = animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uRowTotal].asUInt();

				anim.uNumFrame = animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::uNumFrame].asUInt();
				if (anim.uNumFrame == 0u)
				{
					MessageBoxA(nullptr, "Total number of Animation2D frame is 0.", nullptr, MB_OK);
					return false;
				}

				anim.fFullPlayTime = Pack_float_int(animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::fFullPlayTime].asInt()).f;
				anim.fTimePerFrame = anim.fFullPlayTime / (float)anim.uNumFrame;
				anim.vPivot = Pack_v2_i64(animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::vPivot].asInt64()).v2;

				if (animVal.isMember(RES_INFO::ANIM2D::JSON_KEY::Anim2D::vecFrame))
				{
					for (const auto& vecFrmIter : animVal[RES_INFO::ANIM2D::JSON_KEY::Anim2D::vecFrame])
					{
						anim.vecFrame.emplace_back(vecFrmIter.asUInt());
					}
				}

				m_mapAnim.insert(make_pair(mapIter.key().asString(), anim));
			}
		}
	}

	return true;
}


void CAnim2DAtlas::SetAtlasTexture(Ptr<CTexture> _AtlasTex)
{
	m_AtlasTex = _AtlasTex;
	if (GetKey().empty() && nullptr != m_AtlasTex)
	{
		SetKey(m_AtlasTex->GetKey());
	}
}


void CAnim2DAtlas::SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV)
{
	std::swap(m_vecFrameUV, _vecFrameUV);
}

void CAnim2DAtlas::SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal)
{
	assert(_uColTotal > 0u && _uRowTotal > 0u);
	if (nullptr == m_AtlasTex)
		return;

	m_vecFrameUV.clear();

	m_uColTotal = _uColTotal;
	m_uRowTotal = _uRowTotal;

	float RowSliceUV = 1.f / (float)_uRowTotal;
	float ColSliceUV = 1.f / (float)_uColTotal;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = 0; Col < _uColTotal; ++Col)
	{
		for (UINT Row = 0; Row < _uRowTotal; ++Row)
		{
			tAnimFrameUV uv = {};
			uv.v2_UVLeftTop.x = ColSliceUV * Col;
			uv.v2_UVLeftTop.y = RowSliceUV * Row;

			uv.v2_UVSlice = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}

	//한 프레임의 사이즈를 계산
	m_bRegularFrameSize = true;
	m_v2FrameSize = m_AtlasTex->GetSize() * m_vecFrameUV[0].v2_UVSlice;
}

void CAnim2DAtlas::SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch)
{
	if (nullptr == m_AtlasTex)
		return;

	UINT colend = _uColStart + _uColPitch;
	UINT rowend = _uRowStart + _uRowPitch;

	//조건 확인
	assert(
		0u < _uColPitch
		&& 0u < _uRowPitch
		&& _uColStart <= colend
		&& _uRowStart <= rowend
		&& colend <= _uColTotal
		&& rowend <= _uRowTotal
	);
	
	m_vecFrameUV.clear();

	m_uColTotal = colend - _uColStart;
	m_uRowTotal = rowend - _uRowStart;

	//슬라이스를 구해준다
	float RowSliceUV = 1.f / (float)_uRowTotal;
	float ColSliceUV = 1.f / (float)_uColTotal;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = _uColStart; Col < colend; ++Col)
	{
		for (UINT Row = _uRowStart; Row < rowend; ++Row)
		{
			tAnimFrameUV uv = {};
			uv.v2_UVLeftTop.x = ColSliceUV * Col;
			uv.v2_UVLeftTop.y = RowSliceUV * Row;

			uv.v2_UVSlice = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}

	//한 프레임의 사이즈를 계산
	m_bRegularFrameSize = true;
	m_v2FrameSize = m_AtlasTex->GetSize() * m_vecFrameUV[0].v2_UVSlice;
}

void CAnim2DAtlas::SetNewAnimUV_SC_Redundant(UINT _uRowTotal, UINT _uRowStart, UINT _uRowPitch)
{
	if (nullptr == m_AtlasTex)
		return;

	UINT colend = 17u;
	UINT rowend = _uRowStart + _uRowPitch;

	//조건 확인
	assert(
		0u < _uRowPitch
		&& _uRowStart <= rowend
		&& rowend <= _uRowTotal
	);

	m_vecFrameUV.clear();

	m_uRowTotal = rowend - _uRowStart;

	//슬라이스를 구해준다
	float RowSliceUV = 1.f / (float)_uRowTotal;
	float ColSliceUV = 1.f / (float)colend;

	//Left Top부터 열 순서대로 저장
	//Col은 2단위로 증가
	for (UINT Col = 0u; Col < colend; Col = Col + 2u)
	{
		for (UINT Row = _uRowStart; Row < rowend; ++Row)
		{
			tAnimFrameUV uv = {};
			uv.v2_UVLeftTop.x = ColSliceUV * Col;
			uv.v2_UVLeftTop.y = RowSliceUV * Row;

			uv.v2_UVSlice = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}

	m_uColTotal = 9u;
	m_uRowTotal = rowend - _uRowStart;

	//한 프레임의 사이즈를 계산
	m_bRegularFrameSize = true;
	m_v2FrameSize = m_AtlasTex->GetSize() * m_vecFrameUV[0].v2_UVSlice;
}



tAnim2D* CAnim2DAtlas::AddAnim2D(const string_view _strAnimKey, const tAnim2D& _vecAnimFrameIdx,
	float _fFullPlayTime, eANIM_TYPE _eAnimType, Vec2 _vPivot
)
{
	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, _vecAnimFrameIdx));

	tAnim2D& Anim = pair.first->second;
	Anim.uColTotal = 0u;
	Anim.uRowTotal = 0u;

	Anim.uNumFrame = (UINT)Anim.vecFrame.size();

	Anim.fFullPlayTime = _fFullPlayTime;
	Anim.eAnimType = _eAnimType;
	Anim.vPivot = _vPivot;
	Anim.strKeyAnim2D = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	return &(pair.first->second);
}

tAnim2D* CAnim2DAtlas::AddAnim2D(const string_view _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType, Vec2 _vPivot)
{
	tAnim2D frameIdx = {};
	size_t size = _vecFrame.size();
	for (size_t i = 0; i < size; i++)
	{
		frameIdx.vecFrame.push_back(_vecFrame[i]);
	}

	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, frameIdx));

	tAnim2D& Anim = pair.first->second;
	Anim.uColTotal = 0u;
	Anim.uRowTotal = 0u;

	Anim.uNumFrame = (UINT)Anim.vecFrame.size();

	Anim.fFullPlayTime = _fFullPlayTime;
	Anim.eAnimType = _eAnimType;
	Anim.vPivot = _vPivot;
	Anim.strKeyAnim2D = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	return &(pair.first->second);
}

tAnim2D* CAnim2DAtlas::AddAnim2D(const string_view _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
	float _fFullPlayTime, eANIM_TYPE _eAnimType, Vec2 _vPivot
)
{

	UINT colend = _uColStart + _uColPitch;
	UINT rowend = _uRowStart + _uRowPitch;

	//에러 검사 - 행의 수가 나눠 떨어져야 함
	assert(
			0u < _uColPitch
		&& 0u < _uRowPitch
		&& _uColStart <= colend
		&& _uRowStart <= rowend
	);

	tAnim2D Anim = {};


	for (UINT Col = _uColStart; Col < colend; Col++)
	{
		for (UINT Row = _uRowStart; Row < rowend; Row++)
		{
			Anim.vecFrame.push_back((Col * m_uRowTotal + Row));
		}
	}

	Anim.uColTotal = colend - _uColStart;
	Anim.uRowTotal = rowend - _uRowStart;

	Anim.fFullPlayTime = _fFullPlayTime;
	Anim.eAnimType = _eAnimType; 
	Anim.vPivot = _vPivot;
	Anim.strKeyAnim2D = _strAnimKey;

	switch (Anim.eAnimType)
	{
	case eANIM_TYPE::SEQUENTIAL:
		Anim.uNumFrame = (UINT)Anim.vecFrame.size();
		break;
	case eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP:
		Anim.uNumFrame = Anim.uRowTotal;
		break;
	default:
		break;
	}

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, Anim));

	return &(pair.first->second);
}

tAnim2D* CAnim2DAtlas::AddAnim2D_SC_Redundant(const string_view _strAnimKey, UINT _uRowStart, UINT _uRowPitch, float _fFullPlayTime, Vec2 _vPivot)
{
	assert(
		0 < _uRowPitch 
		&& _uRowStart + _uRowPitch <= m_uRowTotal
		&& 9u == m_uColTotal
	);

	tAnim2D Anim = {};

	UINT _uRowEnd = _uRowStart + _uRowPitch;

	for (UINT Col = 0u; Col < m_uColTotal; Col++)
	{
		for (UINT Row = _uRowStart; Row < _uRowEnd; Row++)
		{
			Anim.vecFrame.push_back((Col * m_uRowTotal + Row));
		}
	}

	Anim.uColTotal = m_uColTotal;
	Anim.uRowTotal = _uRowEnd - _uRowStart;

	Anim.fFullPlayTime = _fFullPlayTime;
	Anim.eAnimType = eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP;
	Anim.vPivot = _vPivot;
	Anim.strKeyAnim2D = _strAnimKey;

	switch (Anim.eAnimType)
	{
	case eANIM_TYPE::SEQUENTIAL:
		Anim.uNumFrame = (UINT)Anim.vecFrame.size();
		break;
	case eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP:
		Anim.uNumFrame = Anim.uRowTotal;
		break;
	default:
		break;
	}

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, Anim));
	return &(pair.first->second);
}

//
tAnim2D* CAnim2DAtlas::AddAnim2D_vecRowIndex(const string_view _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot)
{
	tAnim2D Anim = {};

	size_t rowsize = _vecRow.size();
	for (UINT col = 0; col < m_uColTotal; ++col)
	{
		for (size_t row = 0; row < rowsize; row++)
		{
			Anim.vecFrame.push_back((UINT)(col * m_uRowTotal + _vecRow[row]));
		}
	}

	Anim.uColTotal = m_uColTotal;
	Anim.uRowTotal = (UINT)rowsize;
	Anim.uNumFrame = Anim.uRowTotal;

	Anim.fFullPlayTime = _fFullPlayTime;
	Anim.eAnimType = eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP;
	Anim.vPivot = _vPivot;
	Anim.strKeyAnim2D = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, Anim));
	return &(pair.first->second);
}

const tAnim2D* CAnim2DAtlas::FindAnim2D(const string_view _AnimIdxStrKey)
{
	const auto& iter = m_mapAnim.find(_AnimIdxStrKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return &(iter->second);
}

Vec2 CAnim2DAtlas::GetFrameSize(UINT _uIdxFrameUV) const
{
	if (_uIdxFrameUV > m_vecFrameUV.size())
		return Vec2();

	return m_AtlasTex->GetSize() * m_vecFrameUV[_uIdxFrameUV].v2_UVSlice;
}


