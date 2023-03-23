#include "pch.h"
#include "CAnim2DAtlas.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CStructBuffer.h"

#include <UtilLib_DLL/json/json.h>

CAnim2DAtlas::CAnim2DAtlas()
	: CRes(eRES_TYPE::ANIM2D_ATLAS)
	, m_uColTotal()
	, m_uRowTotal()
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

//TODO : 여기 작성
bool CAnim2DAtlas::SaveJson(Json::Value* _jVal)
{

	return true;
}

bool CAnim2DAtlas::LoadJson(Json::Value* _jVal)
{

	return true;
}


void CAnim2DAtlas::SetAtlasTexture(Ptr<CTexture> _AtlasTex)
{
	m_AtlasTex = _AtlasTex;
}


void CAnim2DAtlas::SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV)
{
	std::swap(m_vecFrameUV, _vecFrameUV);
}

void CAnim2DAtlas::SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal)
{
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
			uv.LeftTopUV.x = ColSliceUV * Col;
			uv.LeftTopUV.y = RowSliceUV * Row;

			uv.SliceUV = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}
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
			uv.LeftTopUV.x = ColSliceUV * Col;
			uv.LeftTopUV.y = RowSliceUV * Row;

			uv.SliceUV = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}
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
			uv.LeftTopUV.x = ColSliceUV * Col;
			uv.LeftTopUV.y = RowSliceUV * Row;

			uv.SliceUV = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}

	m_uColTotal = 9u;
	m_uRowTotal = rowend - _uRowStart;

}



tAnim2D* CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, const tAnim2D& _vecAnimFrameIdx,
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
	Anim.strAnimName = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	return &(pair.first->second);
}

tAnim2D* CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, const vector<UINT>& _vecFrame, float _fFullPlayTime, eANIM_TYPE _eAnimType, Vec2 _vPivot)
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
	Anim.strAnimName = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	return &(pair.first->second);
}

tAnim2D* CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch,
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
	Anim.strAnimName = _strAnimKey;

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

tAnim2D* CAnim2DAtlas::AddAnim2D_SC_Redundant(const string& _strAnimKey, UINT _uRowStart, UINT _uRowPitch, float _fFullPlayTime, Vec2 _vPivot)
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
	Anim.strAnimName = _strAnimKey;

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

tAnim2D* CAnim2DAtlas::AddAnim2D_vecRowIndex(const string& _strAnimKey, const vector<UINT>& _vecRow, float _fFullPlayTime, Vec2 _vPivot)
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
	Anim.strAnimName = _strAnimKey;

	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	auto pair = m_mapAnim.insert(make_pair(_strAnimKey, Anim));
	return &(pair.first->second);
}

const tAnim2D* CAnim2DAtlas::FindAnim2D(const string& _AnimIdxStrKey)
{
	const auto& iter = m_mapAnim.find(_AnimIdxStrKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return &(iter->second);
}


