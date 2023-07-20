#include "pch.h"

#include "cCom_Renderer_TilemapSC.h"

//유닛 로드 및 언로드 용
#include <Engine/cResMgr.h>
#include <Engine/strKey_Default.h>
#include <Engine/cLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/cLayer.h>
#include <Engine/EventDispatcher.h>
#include <Engine/cGameObject.h>
#include <Engine/CDevice.h>
#include <Engine/CConstBuffer.h>
#include <Engine/cRenderMgr.h>
#include <Engine/cTransform.h>
#include <Engine/cCom_Renderer_TilemapComplete.h>

#include <HLSL/S_H_Tilemap_SC.hlsli>

#ifdef _DEBUG
STRKEY strPath_StormLib = "StormLib_DLL_Debug.dll";
#else
STRKEY strPath_StormLib = "StormLib_DLL_Release.dll";
#endif


//#ifdef _DEBUG
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL_Debug")
//#else
//#pragma comment (lib, "StormLib_DLL/StormLib_DLL")
//#endif
//#define STORMLIB_NO_AUTO_LINK
//#include <StormLib_DLL/StormLib.h>

#include "define_SC.h"
#include "CCS_SCMapLoader.h"
#include "cScript_Mineral.h"
#include "strKey_Script.h"
#include "strKey_Shader.h"

#include <Engine/cComMgr.h>

#include <Engine/RandGen.h>
#include <Engine/cCom_Camera.h>

//디버그 출력 확인용
using namespace SC_Map;

cCom_Renderer_TilemapSC::cCom_Renderer_TilemapSC()
	: m_CS_MapLoader()
	, m_bMapLoaded()
	, m_bUnitLoaded()
	, m_eDebugMode()
{
}

cCom_Renderer_TilemapSC::~cCom_Renderer_TilemapSC()
{
}


void cCom_Renderer_TilemapSC::Init()
{
	std::filesystem::path Path(cPathMgr::GetInst()->GetPathRel_Content());
	Path /= DIRECTORY_NAME::SCMAP;
	Path /= DIRECTORY_NAME::SCMAP_TILESET;

	//타일셋 데이터를 저장할 메모리공간 동적할당 
	SC_Map::tTileSet* Tileset = new SC_Map::tTileSet;

	try
	{
		for (int TileSetIdx = 0; TileSetIdx < (int)SC_Map::eTILESET_INFO::END; ++TileSetIdx)
		{
			//타일셋 데이터 초기화
			memset(Tileset, 0, sizeof(SC_Map::tTileSet));
			std::filesystem::path FullPath = Path;

			switch ((eTILESET_INFO)TileSetIdx)
			{
			case SC_Map::eTILESET_INFO::BADLANDS:
				FullPath /= "badlands";
				break;
			case SC_Map::eTILESET_INFO::SPACE_PLATFORM:
				FullPath /= "platform";
				break;
			case SC_Map::eTILESET_INFO::INSTALLATION:
				FullPath /= "install";
				break;
			case SC_Map::eTILESET_INFO::ASH_WORLD:
				FullPath /= "ashworld";
				break;
			case SC_Map::eTILESET_INFO::JUNGLE:
				FullPath /= "jungle";
				break;
			case SC_Map::eTILESET_INFO::DESERT:
				FullPath /= "Desert";
				break;
			case SC_Map::eTILESET_INFO::ICE:
				FullPath /= "Ice";
				break;
			case SC_Map::eTILESET_INFO::TWILIGHT:
				FullPath /= "Twilight";
				break;
			default:
				break;
			}

			FullPath += ".CV5";
			std::ios::openmode mode = std::ios::beg | std::ios::binary | std::ios::in;
			std::ifstream fpCV5(FullPath, mode);
			//_wfopen_s(&fpCV5, FullPath.wstring().c_str(), L"rb");

			FullPath.replace_extension(".VX4");
			std::ifstream fpVX4(FullPath, mode);
			//_wfopen_s(&fpVX4, FullPath.wstring().c_str(), L"rb");

			FullPath.replace_extension(".VR4");
			std::ifstream fpVR4(FullPath, mode);
			//_wfopen_s(&fpVR4, FullPath.wstring().c_str(), L"rb");

			FullPath.replace_extension(".WPE");
			std::ifstream fpWPE(FullPath, mode);
			//_wfopen_s(&fpWPE, FullPath.wstring().c_str(), L"rb");

			FullPath.replace_extension(".VF4");
			std::ifstream fpVF4(FullPath, mode);
			//_wfopen_s(&fpVF4, FullPath.wstring().c_str(), L"rb");

			if (
				false == fpCV5.is_open() ||
				false == fpVX4.is_open() ||
				false == fpVR4.is_open() ||
				false == fpWPE.is_open() ||
				false == fpVF4.is_open()
				)
			{
				fpCV5.close();
				fpVX4.close();
				fpVR4.close();
				fpWPE.close();
				fpVF4.close();

				throw(std::runtime_error("Tileset Data load failed!!"));
			}


			UINT16 cpy = (UINT16)0;
			for (int i = 0; i < CV5_MAX; ++i)
			{
				//각각 2바이트씩 읽어와서 캐스트해서 구조체에 넣어준다.
				fpCV5.read((char*)&cpy, sizeof(UINT16));
				Tileset->cv5[i].TerrainType = (UINT32)cpy;
				cpy = 0;

				fpCV5.read((char*)&cpy, sizeof(UINT16));
				Tileset->cv5[i].Flags = (UINT32)cpy;
				cpy = 0;

				//커서를 16만큼 앞으로 이동한뒤
				fpCV5.seekg(16, std::ios::cur);

				//다시 32바이트를 읽는다.
				fpCV5.read((char*)&(Tileset->cv5[i].MegaTileIndex), sizeof(UINT16) * (size_t)16);

				if (true == fpCV5.eof())
					break;
			}

			fpVX4.read((char*)&(Tileset->vx4), sizeof(VX4) * VX4_MAX);
			fpVR4.read((char*)&(Tileset->vr4), sizeof(VR4) * VR4_MAX);
			fpWPE.read((char*)&(Tileset->wpe), sizeof(WPE) * WPE_MAX);
			fpVF4.read((char*)&(Tileset->vf4), sizeof(VF4) * VF4_MAX);

			//Desc 작성해서 SBuffer 생성
			tSBufferDesc SDesc = {};
			SDesc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_ONLY;
			SDesc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE;

			//타일셋 일괄적으로 동적 할당(2차원 배열)
			m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember = new cStructBuffer[(int)SC_Map::eTILESET_MEMBER::END];

			for (int i = 0; i < (int)SC_Map::eTILESET_MEMBER::END; ++i)
			{
				//0 ~ 5번까지 일치시켜 놓았음.
				SDesc.REGISLOT_t_SRV = i;

				//Desc 설정
				m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].SetDesc(SDesc);

				//각자에게 맞는 구조화버퍼 공간 생성
				switch ((SC_Map::eTILESET_MEMBER)i)
				{
				case SC_Map::eTILESET_MEMBER::CV5:
					m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(CV5), (UINT)CV5_MAX, Tileset->cv5, (UINT)CV5_MAX);

					break;
				case SC_Map::eTILESET_MEMBER::VX4:
					m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VX4), (UINT)VX4_MAX, Tileset->vx4, (UINT)VX4_MAX);

					break;
				case SC_Map::eTILESET_MEMBER::VF4:
					m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VF4), (UINT)VF4_MAX, Tileset->vf4, (UINT)VF4_MAX);

					break;
				case SC_Map::eTILESET_MEMBER::VR4:
					m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(VR4), (UINT)VR4_MAX, Tileset->vr4, (UINT)VR4_MAX);

					break;
				case SC_Map::eTILESET_MEMBER::WPE:
					m_arrpSBufferTileSet[TileSetIdx].arrTileSetMember[i].Create((UINT)sizeof(WPE), (UINT)WPE_MAX, Tileset->wpe, (UINT)WPE_MAX);

					break;

				default:
					break;
				}
			}

			fpCV5.close();
			fpVX4.close();
			fpVR4.close();
			fpWPE.close();
			fpVF4.close();
		}
	}
	catch (const std::runtime_error& error)
	{
		MessageBoxA(nullptr, error.what(), NULL, MB_OK);
		throw(std::runtime_error("Program Terminate"));
	}

	//데이터 저장용 힙 영역을 제거
	delete Tileset;


	m_CS_MapLoader = cResMgr::GetInst()->Load<cComputeShader>(strKey_Shader::Compute::S_C_SCMapLoader);

	assert(nullptr != m_CS_MapLoader);

	Ptr<cGraphicsShader> pShader = cResMgr::GetInst()->Load<cGraphicsShader>(strKey_Shader::Graphics::Tilemap_SC);
	assert(nullptr != pShader);

	GetCurMaterial()->SetShader(pShader);
}

void cCom_Renderer_TilemapSC::Tick()
{
	if (m_bMapLoaded)
	{
		//디버그 모드 설정
		int DebugMode = DEBUGMODE_TILE_NONE;
		if (cRenderMgr::GetInst()->IsEditorCamMode())
			DebugMode = (int)m_eDebugMode;
		GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_INT_DEBUGMODE, &DebugMode);


		//맵 로딩 후 유닛이 로드되지 않았을 경우 유닛 로드
		if (false == m_bUnitLoaded)
		{
			m_bUnitLoaded = true;

			LoadUnit();
		}
	}
}

eRENDER_RESULT cCom_Renderer_TilemapSC::Render()
{
	if (false == m_bMapLoaded)
		return eRENDER_RESULT::FAIL;

	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	cGameObject* pOwner = GetOwner();

	//맵 정보 바인딩
	g_SBufferShareData.iData0 = (int)m_eTileSet;
	g_SBufferShareData.iData1 = (int)GetTileCountX();
	g_SBufferShareData.iData2 = (int)GetTileCountY();

	//맵 데이터 바인딩
	m_MapDataModule.BindDataGS();
	m_MapDataModule.m_pSBuffer_MXTM->BindBufferSRV();
	m_MapDataModule.m_pSBufferRW_Megatile->BindBufferSRV();
	m_MapDataModule.m_pSBufferRW_Minitile->BindBufferSRV();

	return cCom_Renderer_TilemapComplete::Render();
}



bool cCom_Renderer_TilemapSC::LoadMap(const string_view _strMapName)
{
	if (_strMapName == m_strMapName)
		return true;

	UnLoad();

	m_strMapName = _strMapName;

	std::filesystem::path MapPath = cPathMgr::GetInst()->GetPathRel_Content();
	MapPath /= DIRECTORY_NAME::SCMAP;
	MapPath /= m_strMapName;

	std::shared_ptr<SC_Map::tMapRawData> RawData = ExtractMap(MapPath);
	if (nullptr == RawData)
	{
		ERROR_MESSAGE("Failed to Extract Starcraft Map File!");
		return false;
	}

	//szbuffer에 맵의 데이터가 들어와있을 것임.
	//여기서 맵을 그려내는데 필요한 정보를 가져와야 함
	if (false == ReadMapData(RawData))
	{
		ERROR_MESSAGE("Failed to Read Data!");
		return false;
	}

	//맵데이터를 읽어오는것까지 성공했을 경우 CS로 데이터를 업로드한다.
	if (false == UploadMapDataToCS())
	{
		ERROR_MESSAGE("Failed to Upload Map Data to GPU!");
		return false;
	}

	//타겟이 될 텍스처를 동적할당하고, UAV에 바인딩
	m_pMapTex = new cTexture;
	UINT BindFlag = D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

	UINT32 uDataX = 32u * GetTileCountX();
	UINT32 uDataY = 32u * GetTileCountY();
	m_pMapTex->Create(uDataX, uDataY, DXGI_FORMAT_B8G8R8A8_UNORM, BindFlag, D3D11_USAGE::D3D11_USAGE_DEFAULT);
	m_pMapTex->BindData_UAV(REGISLOT_u_TEXTURERW_TARGET);

	//맵 이름으로 키를 설정한다.
	m_pMapTex->SetKey(m_strMapName);

	//필요한 그룹의 수 계산
	m_CS_MapLoader->CalcGroupNumber(tNumDataCS{ uDataX, uDataY, 1u });
	m_CS_MapLoader->Execute();

	UnBind();

	return bLoaded;
}

void CCS_SCMapLoader::UnBind()
{
	//타일셋 Unbind
	for (int i = 0; i < (int)SC_Map::eTILESET_MEMBER::END; ++i)
	{
		m_arrpSBufferTileSet[(int)m_pMapWorkSpace->eTileSet].arrTileSetMember[i].UnBind();
	}


	//맵 데이터를 CPU 메모리로 복사 후 구조화 버퍼를 언바인드
	UINT numTile = m_pMapWorkSpace->uNumMegatileX * m_pMapWorkSpace->uNumMegatileY;
	m_pMapWorkSpace->vecMegaTile.resize(numTile);
	m_pMapWorkSpace->pSBufferRW_Megatile->GetData(m_pMapWorkSpace->vecMegaTile.data(), sizeof(tMegaTile) * numTile);
	m_pMapWorkSpace->pSBufferRW_Megatile->UnBind();



	//Walkability 데이터 가져온 뒤 제거
	numTile *= 16;
	m_pMapWorkSpace->vecMiniTile.resize(numTile);
	m_pMapWorkSpace->pSBufferRW_Minitile->GetData(m_pMapWorkSpace->vecMiniTile.data(), sizeof(tMiniTile) * numTile);
	m_pMapWorkSpace->pSBufferRW_Minitile->UnBind();

	//텍스처도 UAV 모드를 Unbind
	m_pMapWorkSpace->pMapTex->UnBind();

	if (true == m_bMapLoaded)
	{
		Tilemap()->GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, m_pMapData->m_pMapTex);

		cTransform& pTF = Transform();

		g_GlobalVal.fMapSizeX = (float)m_pMapData->uNumMegatileX * 32.f;
		g_GlobalVal.fMapSizeY = (float)m_pMapData->uNumMegatileY * 32.f;
		
		pTF.SetSize(Vec3(g_GlobalVal.fMapSizeX, g_GlobalVal.fMapSizeY, 1.f));

		//#define MTRL_SCALAR_VEC2_MAPSIZE	  MTRLDATA_PARAM_SCALAR(VEC2, 2)
		{
			Vec2 MapSize = Vec2(g_GlobalVal.fMapSizeX, g_GlobalVal.fMapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MAPSIZE, &MapSize);
		}

		UINT MapSizeX = m_pMapData->uNumMegatileX;
		UINT MapSizeY = m_pMapData->uNumMegatileY;

		//#define MTRL_SCALAR_VEC2_MEGATILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 0)
		{
			Vec2 MapSize = Vec2(MapSizeX, MapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MEGATILESIZE, &MapSize);
		}

		//#define MTRL_SCALAR_VEC2_MINITILESIZE MTRLDATA_PARAM_SCALAR(VEC2, 1)
		{
			//메가타일에는 각 미니타일 4개씩이 들어있다.
			MapSizeX *= 4u;
			MapSizeY *= 4u;
			Vec2 MapSize = Vec2(MapSizeX, MapSizeY);
			GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MINITILESIZE, &MapSize);
		}
		



		//스타크래프트 맵의 좌표는 Left Top이 원점이므로
		//맵의 좌측 상단을 원점으로 맞춰준다(2사분면에 집어넣어준다)
		//y축만 음수로 뒤집으면 되니까 좌표 맞추기가 편해짐
		Vec2 MapPos = Vec2(g_GlobalVal.fMapSizeX, -g_GlobalVal.fMapSizeY);
		MapPos /= 2.f;
		
		pTF.SetRelativePosXY(MapPos);
	}

	m_pSBuffer_MXTM->BindBufferSRV();
	m_pSBufferRW_Megatile->BindBufferSRV();
	m_pSBufferRW_Minitile->BindBufferSRV();

	return m_bMapLoaded;
}

std::shared_ptr<tMapRawData> cCom_Renderer_TilemapSC::ExtractMap(const std::filesystem::path& _MapFilePath)
{
	HANDLE hMpq = NULL;          // Open archive handle
	HANDLE hFile = NULL;          // Archived file handle
	//HANDLE handle = NULL;          // Disk file handle

	//DLL 로드
	HMODULE DLLModule = LoadLibraryW(std::filesystem::path(strPath_StormLib).wstring().c_str());

	if (nullptr == DLLModule)
	{
		ERROR_MESSAGE("StormLib Load Failed!");
		return nullptr;
	}

	//함수 등록
	//MPQ 파일 로드
	typedef bool(WINAPI* pSFileOpenArchive)(const TCHAR*, DWORD, DWORD, HANDLE*);
	pSFileOpenArchive MpqOpenFunc = (pSFileOpenArchive)GetProcAddress(DLLModule, "SFileOpenArchive");

	typedef bool(WINAPI* pSFileOpenFileEx)(HANDLE, const char*, DWORD, HANDLE*);
	pSFileOpenFileEx funcOpenScenarioChk = (pSFileOpenFileEx)GetProcAddress(DLLModule, "SFileOpenFileEx");

	//DWORD WINAPI SFileGetFileSize(HANDLE hFile, LPDWORD pdwFileSizeHigh);
	typedef DWORD(WINAPI* pSFileGetFileSize)(HANDLE, LPDWORD);
	pSFileGetFileSize funcGetFileSize = (pSFileGetFileSize)GetProcAddress(DLLModule, "SFileGetFileSize");

	//bool WINAPI SFileReadFile(HANDLE hFile, void* pvBuffer, DWORD dwToRead, LPDWORD pdwRead, LPOVERLAPPED lpOverlapped);
	typedef bool (*pSFileReadFile)(HANDLE, void*, DWORD, LPDWORD, LPOVERLAPPED);
	pSFileReadFile funcReadFile = (pSFileReadFile)GetProcAddress(DLLModule, "SFileReadFile");

	//bool WINAPI SFileCloseFile(HANDLE hFile)
	typedef bool (WINAPI* pSFileCloseFile)(HANDLE);
	pSFileCloseFile funcCloseFile = (pSFileCloseFile)GetProcAddress(DLLModule, "SFileCloseFile");

	//bool WINAPI SFileCloseArchive(HANDLE hMpq)
	typedef bool (WINAPI* pSFileCloseArchive)(HANDLE);
	pSFileCloseArchive funcCloseArchive = (pSFileCloseArchive)GetProcAddress(DLLModule, "SFileCloseArchive");


	//DLL로부터 로드 실패 또는 함수에서 로드를 실패했을 경우 false 반환
	if (nullptr == MpqOpenFunc || false == MpqOpenFunc(_MapFilePath.wstring().c_str(), 0, 0, &hMpq))
	{
		int nError = GetLastError();
		string Err = "ErrCode: ";
		Err += std::to_string(nError);
		ERROR_MESSAGE(Err.c_str());
		return nullptr;
	}

	//MPQ 파일 내부의 scenario.chk 파일 탐색.
	if (nullptr == funcOpenScenarioChk || false == funcOpenScenarioChk(hMpq, "staredit\\scenario.chk", 0, &hFile))
	{
		int nError = GetLastError();
		string Err = "ErrCode: ";
		Err += std::to_string(nError);
		ERROR_MESSAGE(Err.c_str());
		return nullptr;
	}


	// Read the file from the archive
	shared_ptr<SC_Map::tMapRawData> pMap = std::make_shared<tMapRawData>();

	//파일의 사이즈를 받아온 후 해당 파일을 받아올 공간을 동적할당 및 초기화한다.
	pMap->Size = funcGetFileSize(hFile, NULL);
	pMap->pData = new char[pMap->Size];
	memset(pMap->pData, 0, (size_t)pMap->Size);

	//데이터를 읽어온다.
	bool Suc = funcReadFile(hFile, pMap->pData, pMap->Size, &pMap->ActualReadByte, NULL);

	//파일 연결 해제
	funcCloseFile(hFile);
	funcCloseArchive(hMpq);

	//라이브러리 연결 해제
	FreeLibrary(DLLModule);

	if (false == Suc)
	{
		int nError = GetLastError();
		string Err = "ErrCode: ";
		Err += std::to_string(nError);
		ERROR_MESSAGE(Err.c_str());
		return nullptr;
	}

	return pMap;
}

bool cCom_Renderer_TilemapSC::ReadMapData(std::shared_ptr<tMapRawData> _RawData)
{
	using namespace std;
	//tMapDataChunk 자체의 데이터 사이즈는 크지 않으므로 스택에 생성
	shared_ptr<tMapDataChunk> arrMapDataChunk[(int)eSCMAP_DATA_TYPE::END] = {};

	
	//읽어온 데이터를 string에 옮긴다.
	std::string_view View(_RawData->pData, _RawData->Size);
	string DataStr(View);

	//멀티스레드 연습용
	vector<future<shared_ptr<tMapDataChunk>>> vecFuture;
	for (int i = 0; i < (int)eSCMAP_DATA_TYPE::END; ++i)
	{
		vecFuture.emplace_back(std::async(std::launch::async, &cCom_Renderer_TilemapSC::MultiThread_CopyChunk, this, std::cref(DataStr), (eSCMAP_DATA_TYPE)i));
	}
	for (int i = 0; i < (int)eSCMAP_DATA_TYPE::END; ++i)
	{
		arrMapDataChunk[i] = vecFuture[i].get();
	}


	//맵 사이즈 읽기
	if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE]->length != 4)
		return false;

	UINT MegaTileCountX = (UINT)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE]->Data);
	UINT MegaTileCountY = (UINT)*(unsigned short*)(arrMapDataChunk[(int)eSCMAP_DATA_TYPE::MAPSIZE]->Data + 2);

	SetTileCount(MegaTileCountX, MegaTileCountY);

	Vec2 vMapSize = Vec2(MegaTileCountX, MegaTileCountY);
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_VEC2_MAPSIZE, &vMapSize);

	//MXTM(TILEMAP_ATLAS) 생성 및 전송
	tSBufferDesc SBufferDesc = {};
	SBufferDesc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_ONLY;
	SBufferDesc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__COMPUTE;
	SBufferDesc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_MXTM;

	m_pSBuffer_MXTM = std::make_unique<cStructBuffer>();
	m_pSBuffer_MXTM->SetDesc(SBufferDesc);

	UINT DataCount = (UINT)arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP_ATLAS]->length / 16u;
	m_pSBuffer_MXTM->Create((UINT)sizeof(MXTM), DataCount, arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TILEMAP_ATLAS]->Data, DataCount);
	m_pSBuffer_MXTM->BindBufferSRV();

	//지형 파일 읽기
	if (arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN]->length != 2)
		return false;

	//첫 1바이트에 지형 정보가 들어있다.
	unsigned char Info = arrMapDataChunk[(int)eSCMAP_DATA_TYPE::TERRAIN]->Data[0];

	//앞의 4비트는 0으로 변환
	unsigned char bitshift = 0b00001111;
	Info &= bitshift;

	//혹시나 비트시프트 이후 8을 초과하는 값이 나올 경우 8을 빼준다.
	if (Info >= (unsigned char)8)
		Info -= (unsigned char)8;

	//타일셋 정보를 등록
	m_eTileSet = (eTILESET_INFO)Info;

	//유닛 정보
	//유효성 체크(데이터가 36바이트 단위이므로 36바이트로 나누어 떨어져야 함
	if (0 != arrMapDataChunk[(int)eSCMAP_DATA_TYPE::UNIT_PLACEMENT]->length % sizeof(SC_Map::tUnitData))
		return false;

	size_t NumData = arrMapDataChunk[(int)eSCMAP_DATA_TYPE::UNIT_PLACEMENT]->length / sizeof(SC_Map::tUnitData);

	//저장된 데이터를 형변환한 후 데이터를 옮긴다.
	tUnitData* pUnitData = (tUnitData*)(arrMapDataChunk)[(int)eSCMAP_DATA_TYPE::UNIT_PLACEMENT]->Data;
	m_vecUnitData = vector<SC_Map::tUnitData>(pUnitData, pUnitData + NumData);

	return true;
}

std::shared_ptr<SC_Map::tMapDataChunk> cCom_Renderer_TilemapSC::MultiThread_CopyChunk(const std::string& _dataStr, SC_Map::eSCMAP_DATA_TYPE _eDataType)
{
	using namespace std;
	shared_ptr<tMapDataChunk> pData = make_shared<tMapDataChunk>();

	//위치 찾기
	size_t pos = 0;
	pos = _dataStr.find(strKey_SCMap[(int)_eDataType]);

	if (pos == string::npos)
		return nullptr;

	{
		const char* adress = _dataStr.data() + pos;

		//이름 복사
		memcpy(pData->TypeName, adress, 4);

		//데이터 덩어리 사이즈 복사
		int* len = (int*)(adress + 4);
		pData->length = *len;

		pData->Data = new unsigned char[pData->length];
		memcpy(pData->Data, adress + 8, pData->length);
	}


	return pData;
}

bool cCom_Renderer_TilemapSC::UploadMapDataToCS()
{
	//타일셋을 바인딩해준다.
	for (int i = 0; i < (int)SC_Map::eTILESET_MEMBER::END; ++i)
	{
		m_arrpSBufferTileSet[(int)m_eTileSet].arrTileSetMember[i].BindBufferSRV();
	}

	tSBufferDesc Desc = {};

	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_WRITE;

	UINT numTile = GetTileCountX() * GetTileCountY();
	{
		//MEGATILE
		using namespace define_Shader::ePIPELINE_STAGE_FLAG;
		UINT flagPipeline = __VERTEX | __PIXEL | __COMPUTE;
		Desc.flag_PipelineBindTarget_SRV = flagPipeline;
		Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_MEGATILE;
		Desc.REGISLOT_u_UAV = REGISLOT_u_SBUFFERRW_MEGATILE;

		//Megatile 정보를 보내고 받아올 구조화 버퍼를 생성한다.
		m_pSBufferRW_Megatile = std::make_unique<cStructBuffer>();
		m_pSBufferRW_Megatile->SetDesc(Desc);
		m_pSBufferRW_Megatile->Create(sizeof(tMegaTile), numTile, nullptr, 0u);
		m_pSBufferRW_Megatile->BindBufferUAV();
	}


	{
		//Minitile
		Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_MINITILE;
		Desc.REGISLOT_u_UAV = REGISLOT_u_SBUFFERRW_MINITILE;

		m_pSBufferRW_Minitile = std::make_unique<cStructBuffer>(Desc);

		//메가타일 하나당 16개의 미니타일이 존재
		numTile *= 16;
		m_pSBufferRW_Minitile->Create(sizeof(tMiniTile), numTile, nullptr, 0u);
		m_pSBufferRW_Minitile->BindBufferUAV();
	}
	return true;
}

void cCom_Renderer_TilemapSC::UnLoad()
{
	UnLoadUnit();

	GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, nullptr);
	m_pMapTex = nullptr;

	SetTileCount(0u, 0u);
	m_eTileSet = eTILESET_INFO::END;

	m_strMapName = "";
	m_vecMegaTile.clear();
	m_vecMiniTile.clear();

	m_pSBuffer_MXTM = nullptr;
	m_pSBufferRW_Megatile = nullptr;
	m_pSBufferRW_Minitile = nullptr;

	m_bMapLoaded = false;
}


void cCom_Renderer_TilemapSC::LoadUnit()
{
	using namespace SC_Map;
	using namespace SC;

	size_t size = m_vecUnitData.size();
	for (size_t i = 0; i < size; ++i)
	{
		const tUnitData& unit = m_vecUnitData[i];

		Ptr<CPrefab> UnitPrefab = cResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName((SC::eUNIT_ID)unit.ID));

		//유닛 생성. Y좌표계는 반전해줘야 함
		cGameObject* SpawnedObj = nullptr;
		if (nullptr != UnitPrefab)
		{
			SpawnedObj = EventDispatcher::SpawnPrefab2D(UnitPrefab, Vec2((float)unit.PosX, -(float)unit.PosY));
		}


		//유닛에 따라서 추가적인 작업을 해야 할 경우 여기서 해줄것
		switch (unit.ID)
		{
		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_1:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 0;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;

		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_2:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 1;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::MINERAL_FIELD_TYPE_3:
		{
			if (nullptr == SpawnedObj)
				break;
			//미네랄 스프라이트 설정
			int MineralType = 2;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &MineralType);

			//미네랄 남은 자원량 설정
			cScript_Mineral* pScriptMineral = static_cast<cScript_Mineral*>(SpawnedObj->ScriptHolder()->FindScript(strKey_Script::cScript_Mineral));
			pScriptMineral->SetMineralLeft((UINT)unit.Resources);

			break;
		}

		case (UINT16)SC::eUNIT_ID::VESPENE_GEYSER:
		{
			if (nullptr == SpawnedObj)
				break;
			float CurTileSet = (float)m_pMapData->eTileSet;
			SpawnedObj->SetMtrlScalarParam(MTRL_SCALAR_FLOAT_VESPINE_SPRITE, &CurTileSet);
		}
		break;

		case (UINT16)SC::eUNIT_ID::START_LOCATION:
		{
			m_vecStartLocation.push_back(Vec2((float)unit.PosX, -(float)unit.PosY));
		}


		break;


		default:
			break;
		}

	}


	StartLocation();
}

void cCom_Renderer_TilemapSC::UnLoadUnit()
{
	if (false == m_bUnitLoaded)
		return;

	m_vecStartLocation.clear();
	{
		cLayer& ResourceLayer = cLevelMgr::GetInst()->GetCurLevel()->GetLayer(SC::LAYER_INFO::Resource);
		const vector<cGameObject*>& vecObj = ResourceLayer.GetvecObj();
		size_t size = vecObj.size();
		for (size_t i = 0; i < size; ++i)
		{
			EventDispatcher::DestroyGameObj(vecObj[i]);
		}
	}

	{
		cLayer& ResourceLayer = cLevelMgr::GetInst()->GetCurLevel()->GetLayer(SC::LAYER_INFO::GroundUnitMain);
		const vector<cGameObject*>& vecObj = ResourceLayer.GetvecObj();
		size_t size = vecObj.size();
		for (size_t i = 0; i < size; ++i)
		{
			EventDispatcher::DestroyGameObj(vecObj[i]);
		}
	}

	m_bUnitLoaded = false;
}

void cCom_Renderer_TilemapSC::StartLocation()
{
	if (m_vecStartLocation.empty())
		return;

	//스타팅 포인트 중 랜덤한 곳에 커맨드 생성
	int StartPos = (int)m_vecStartLocation.size();
	StartPos = RandGen::GetRand(0, StartPos - 1);
	{
		Ptr<CPrefab> Command = cResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_COMMAND_CENTER));
		assert(nullptr != Command);

		EventDispatcher::SpawnPrefab2D(Command, m_vecStartLocation[StartPos]);

		cCom_Camera* pMainCam = cRenderMgr::GetInst()->GetCamera(eCAMERA_INDEX::MAIN);
		assert(pMainCam);
		pMainCam->GetOwner()->Transform().SetRelativePosXY(m_vecStartLocation[StartPos]);

		Ptr<CPrefab> Marine = cResMgr::GetInst()->Load<CPrefab>(SC::GetUnitName(SC::eUNIT_ID::TERRAN_MARINE));

		assert(nullptr != Marine);

		EventDispatcher::SpawnPrefab2D(Marine, m_vecStartLocation[StartPos] + Vec2(100.f, 100.f));
	}
}

tNumDataCS cCom_Renderer_TilemapSC::cMapDataModule::BindDataCS()
{
	return tNumDataCS();
}

bool cCom_Renderer_TilemapSC::cMapDataModule::BindDataGS()
{
	return false;
}

void cCom_Renderer_TilemapSC::cMapDataModule::UnBind()
{
}
