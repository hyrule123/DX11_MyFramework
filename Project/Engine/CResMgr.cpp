#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "strKeyDefaultRes.h"

#include "CCS_SetColor.h"
#include "CCS_ParticleUpdate.h"
#include "CCS_SCMapLoader.h"
#include "CCS_Initialize.h"

#include "CAnim2DAtlas.h"

#include <fstream>


CResMgr::CResMgr()
	: m_bResUpdated(true)
{
	
}

CResMgr::~CResMgr()
{
}



void CResMgr::init()
{
	CreateResClassTypeIndex();

	CreateDefaultMesh();
	
	CreateDefaultShader();

	CreateDefaultMaterial();

	LoadDefaultTexture();

	CreateDefaultAnimAtlas();
}

void CResMgr::CreateResClassTypeIndex()
{
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESH));
	//m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESHDATA));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), eRES_TYPE::MATERIAL));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), eRES_TYPE::TEXTURE));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CAnim2DAtlas)), eRES_TYPE::ANIM2D_ATLAS));
	//m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CSound)), eRES_TYPE::SOUND));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CPrefab)), eRES_TYPE::PREFAB));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CGraphicsShader)), eRES_TYPE::GRAPHICS_SHADER));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CComputeShader)), eRES_TYPE::COMPUTE_SHADER));
}

void CResMgr::CreateDefaultMesh()
{	
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	Ptr<CMesh> pMesh = nullptr;


	// ==============
	// PointMesh 생성
	// ==============
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	//v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	
	UINT idx = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes(DEFAULT_RES::MESH::POINT, pMesh);
	pMesh = nullptr;


	// =============
	// RectMesh 생성
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.0f);
	//v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.0f);
	//v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(DEFAULT_RES::MESH::RECT, pMesh);
	pMesh = nullptr;

	//Debug Rect Mesh
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(DEFAULT_RES::MESH::DEBUG_RECT, pMesh);
	//============================


	//===================
	//CircleMesh
	//===================
	pMesh = nullptr;

	//정점버퍼와 인덱스버퍼 초기화
	vecVtx.clear();
	vecIdx.clear();

	//반지름과 조각별 각도를 정하고 각도를 계산.
	float radius = 0.5f;
	int fslice = 40;
	float AngleStride = XM_2PI / fslice;

	//먼저 원의 중심점을 정점버퍼에 먼저 추가(중심점 (0.5f, 0.5f)에 반지름이 0.5f인 원을 추가할 예정)
	v.vPos = Vec3(0.f, 0.f, 0.f);
	//v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	//반복 수 ' - 1 '만큼 반복(마지막 정점은 두 번째 정점을 사용하면 된다.

	//원의 Pos값(정점의 위치)과 UV값은 1:1 대응이 되지 않음
	//y축은 서로 방향이 반대이고 중심점 좌표도 차이가 남.
	/*					<POS>
					(0.f, 0.5f)
				/					\
	(-0.5f, 0.f)	(0.f, 0.f)		(0.5f, 0.f)
				\					/
					(0.f, -0.5f)

						<UV>
					(0.5f, 0.f)
				/					\
	(0.f, 0.5f)		(0.5f, 0.5f)	(1.f, 0.5f)
				\					/
					(0.5f, 1.f)
	*/

	//오른쪽 끝부터 피자조각 모양으로 정점버퍼를 추가한다.
	//마지막 전까지 정점을 계산해서 정점버퍼에 삽입한다.
	//배열의 0번 인덱스에는 중심점이 들어가 있으므로 1부터 시작하면 i를 인덱스버퍼의 인덱스로 사용 가능
	for (int i = 0; i < fslice; ++i)
	{
		v.vPos.x = radius * cosf(AngleStride * i);
		v.vPos.y = radius * sinf(AngleStride * i);
		
		//UV는 중점 기준으로 더하거나 빼는 방식으로 해준다.
		v.vUV.x = 0.5f + v.vPos.x;
		v.vUV.y = 0.5f -(v.vPos.y);	//반대 방향

		vecVtx.push_back(v);

		//정점배열의 첫번째는 중심점이 들어가 있으므로 i + 1 == 현재 정점버퍼의 사이즈가 된다
		//인덱스는 시계 방향으로 삽입
		//마지막 인덱스는 따로 직접 삽입
		if (i == (fslice - 1))
			continue;

		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back((UINT)vecVtx.size() - 1u);
	

	//만든 정점버퍼와 인덱스버퍼를 메쉬에 삽입한다.
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(DEFAULT_RES::MESH::CIRCLE, pMesh);
	pMesh = nullptr;


	//디버그 메쉬
	vecIdx.clear();
	for (int i = 0; i < fslice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(DEFAULT_RES::MESH::DEBUG_CIRCLE, pMesh);
}

void CResMgr::CreateDefaultShader()
{
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
}


bool CResMgr::CreateDefaultGraphicsShader()
{
	// ============
	// Debug Shader
	// Topology: LineStrip
	// Rasterizer: No Culling
	// Blend State: Default
	// Shader Domain: Opaque
	// ============

	std::filesystem::path GSDir(RELATIVE_PATH::SHADER_GRAPHICS::A);

	//Shader 폴더 순회돌면서 전부 로드하는 코드
	std::filesystem::directory_iterator GSDirIter(GSDir);

	for (; false == GSDirIter._At_end(); ++GSDirIter)
	{
		if (false == GSDirIter->is_directory())
		{
			const string& fileName = GSDirIter->path().extension().string();

			//.json 파일일 경우 동적할당하고 로드한다.
			if (string::npos != fileName.find(JSON_SHADERINFO::strJsonExtension))
			{
				Ptr<CGraphicsShader> pGS = new CGraphicsShader;
				pGS->Load(GSDirIter->path().filename());

				CResMgr::GetInst()->AddRes<CGraphicsShader>(pGS->GetKey(), pGS);
			}
		}
	}

	return true;
	
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::DEBUG;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	

	//	//pShader->SetKey(DEFAULT_RES::SHADER::GRAPHICS::DEBUG);
	//	//pShader->CreateShader((void*)g_VS_Debug, sizeof(g_VS_Debug), eSHADER_TYPE::__VERTEX);
	//	//pShader->CreateShader((void*)g_PS_Debug, sizeof(g_PS_Debug), eSHADER_TYPE::__PIXEL);
	//	//pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//	//pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
	//	//pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_TEST_NO_WRITE);
	//	//pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	
	//}



	////// ===========
	////// Test Shader
	////// =========== 
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::TEST;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	
	//	//pShader->SetKey(DEFAULT_RES::SHADER::TEST);
	//	//pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADER_TYPE::__VERTEX);
	//	//pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADER_TYPE::__PIXEL);
	//	//pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	//pShader->SetBlendState(eBLENDSTATE_TYPE::DEFAULT);
	//	//pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	//AddRes(pShader->GetKey(), pShader);
	//}

	////// ============
	////// std2D Shader
	////// ============
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::STD2D;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(DEFAULT_RES::SHADER::STD2D);
	////	pShader->CreateShader((void*)g_VS_std2D, sizeof(g_VS_std2D), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_PS_std2D, sizeof(g_PS_std2D), eSHADER_TYPE::__PIXEL);
	////	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	////	AddRes(pShader->GetKey(), pShader);
	//}


	////// ==================
	////// std2D_Light Shader
	////// ==================
	////// 광원을 처리할 수 있는 2D 쉐이더
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::STD2DLIGHT;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(DEFAULT_RES::SHADER::STD2D_LIGHT);
	////	pShader->CreateShader((void*)g_VS_std2D_Light, sizeof(g_VS_std2D_Light), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_PS_std2D_Light, sizeof(g_PS_std2D_Light), eSHADER_TYPE::__PIXEL);
	////	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	////	AddRes(pShader->GetKey(), pShader);
	//}



	////// ===============================
	////// Tilemap_Atlas
	////// RS_TYPE : CULL_BACK
	////// DS_TYPE : LESS(Default)
	////// BS_TYPE : MASK

	////// Parameter
	////// g_CBuffer_Mtrl_Scalar.INT_0 : Tile X Count
	////// g_CBuffer_Mtrl_Scalar.int_1 : Tile Y Count
	////// g_tex_0 : Tile Atlas Texture
	//////===============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::TILEMAPATLAS;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(DEFAULT_RES::SHADER::TILEMAP_ATLAS);
	////	pShader->CreateShader((void*)g_VS_Tilemap_Atlas, sizeof(g_VS_Tilemap_Atlas), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_PS_Tilemap_Atlas, sizeof(g_PS_Tilemap_Atlas), eSHADER_TYPE::__PIXEL);
	////	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	////	pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
	////	pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
	////	AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//}

	////// ===============================
	////// Tilemap_Atlas
	////// RS_TYPE : CULL_BACK
	////// DS_TYPE : LESS(Default)
	////// BS_TYPE : MASK

	////// Parameter
	////// g_tex_0 : Tile Atlas Texture
	//////===============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::TILEMAPCOMPLETE;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	//	pShader->SetKey(DEFAULT_RES::SHADER::TILEMAP_COMPLETE);
	//	//	pShader->CreateShader((void*)g_VS_Tilemap_Complete, sizeof(g_VS_Tilemap_Complete), eSHADER_TYPE::__VERTEX);
	//	//	pShader->CreateShader((void*)g_PS_Tilemap_Complete, sizeof(g_PS_Tilemap_Complete), eSHADER_TYPE::__PIXEL);
	//	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	//	pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
	//	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
	//	//	AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//}



	////// ============================
	////// ParticleRender
	////// 
	////// RS_TYPE : CULL_NONE
	////// DS_TYPE : NO_WRITE
	////// BS_TYPE : ALPHA_BLEND

	////// Parameter
	////// g_CBuffer_Mtrl_Scalar.INT_0 : Particle Index
	////// 
	////// Domain : TRANSPARENT
	////// ============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / DEFAULT_RES::SHADER::GRAPHICS::PARTICLE;
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(DEFAULT_RES::SHADER::PARTICLE_RENDER);
	////
	////	pShader->CreateShader((void*)g_VS_Particle, sizeof(g_VS_Particle), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_GS_Particle, sizeof(g_GS_Particle), eSHADER_TYPE::__GEOMETRY);
	////	pShader->CreateShader((void*)g_PS_Particle, sizeof(g_PS_Particle), eSHADER_TYPE::__PIXEL);

	////	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	////	pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_WRITE);
	////	pShader->SetBlendState(eBLENDSTATE_TYPE::ALPHA_BLEND);
	////	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	////	pShader->SetShaderDomain(eSHADER_DOMAIN::_TRANSPARENT); //알파 블렌딩을 사용하므로


	////	AddRes(pShader->GetKey(), pShader);
	//}
}







bool CResMgr::CreateDefaultComputeShader()
{

	//컴퓨트쉐이더는 클래스에 종속적이므로 수동으로 로드해줘야 한다.

	Ptr<CComputeShader> pCS = new CCS_Initialize;
	if (true == pCS->Load(DEFAULT_RES::SHADER::COMPUTE::INITALIZE))
	{
		AddRes(pCS->GetKey(), pCS);
		pCS->Execute();
	}
	pCS = nullptr;


	pCS = new CCS_SetColor(32u, 32u, 1u);

	if (true == pCS->Load(DEFAULT_RES::SHADER::COMPUTE::SETCOLOR))
	{
		AddRes(pCS->GetKey(), pCS);
	}
	pCS = nullptr;

	//기본 파티클 쉐이더
	pCS = new CCS_ParticleUpdate(128u, 1u, 1u);

	if (true == pCS->Load(DEFAULT_RES::SHADER::COMPUTE::PARTICLEBASIC))
	{
		AddRes(pCS->GetKey(), pCS);
	}
	pCS = nullptr;

	//비 효과 파티클 쉐이더
	pCS = new CCS_ParticleUpdate(128u, 1u, 1u);
	pCS = new CCS_ParticleUpdate(128u, 1u, 1u);
	
	if (true == pCS->Load(DEFAULT_RES::SHADER::COMPUTE::PARTICLERAINDROP))
	{
		AddRes(pCS->GetKey(), pCS);
	}
	pCS = nullptr;

	pCS = new CCS_SCMapLoader;
	
	if (true == pCS->Load(DEFAULT_RES::SHADER::COMPUTE::SCMAPLOADER))
	{
		AddRes(pCS->GetKey(), pCS);
	}
	pCS = nullptr;


	return true;
}



void CResMgr::CreateDefaultMaterial()
{
	//Debug Material
	{
		//Rectangle
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::DEBUG));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::DEBUG_RECT);
		AddRes(pMtrl->GetKey(), pMtrl);

		//Circle
		pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::DEBUG));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::DEBUG_CIRCLE);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Test Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::TEST));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::TEST);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2D Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::STD2D));
		pMtrl->SetInstancedRender(true);

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::STD2D);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2DLight Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		//pMtrl->SetInstancedRender(true);
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::STD2DLIGHT));

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::STD2D_LIGHT);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Tilemap Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::TILEMAPATLAS));

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::TILEMAP_ATLAS);
		AddRes(pMtrl->GetKey(), pMtrl);
	}
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::TILEMAPCOMPLETE));

		pMtrl->SetKey(DEFAULT_RES::MATERIAL::TILEMAP_COMPLETE);
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	//Particle Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(DEFAULT_RES::SHADER::GRAPHICS::PARTICLE));
		pMtrl->SetKey(DEFAULT_RES::MATERIAL::PARTICLE_RENDER);
		AddRes(pMtrl->GetKey(), pMtrl);
	}
}

void CResMgr::LoadDefaultTexture()
{
	Load<CTexture>(DEFAULT_RES::TEXTURE::FIGHTER, "Fighter.bmp");
	Load<CTexture>(DEFAULT_RES::TEXTURE::HOS,  "HOS.png");

	Load<CTexture>(DEFAULT_RES::TEXTURE::BEHEADED_ATLAS,  "beheaded.png");
	Load<CTexture>(DEFAULT_RES::TEXTURE::BEHEADED_ATLAS_NORMAL,  "beheaded_n.png");

	Load<CTexture>(DEFAULT_RES::TEXTURE::TILE_ATLAS, "TILE.bmp");
	Load<CTexture>(DEFAULT_RES::TEXTURE::LINK_ATLAS, "link.png");

	Load<CTexture>(DEFAULT_RES::TEXTURE::MARINE_ATLAS, "marine.bmp");

	//노이즈텍스처
	Load<CTexture>(DEFAULT_RES::TEXTURE::NOISE_TEXTURE_0, "noise/noise_01.png");
}

void CResMgr::CreateDefaultAnimAtlas()
{
	//{
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
	//	Atlas->SetAtlasTexture(FindRes<CTexture>(DEFAULT_RES::TEXTURE::REAVER_ATLAS));

	//	Atlas->SetNewAnimUV_SC_Redundant(9u, 0u, 9u);
	//	Atlas->AddAnim2D_SC_Redundant(DEFAULT_RES::ANIM2D::REAVERMOVE, 0u, 9u, 3.f);

	//	AddRes<CAnim2DAtlas>(DEFAULT_RES::TEXTURE::REAVER_ATLAS, Atlas);
	//}

	//{
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
	//	Atlas->SetAtlasTexture(FindRes<CTexture>(DEFAULT_RES::TEXTURE::CORSAIR_ATLAS));

	//	Atlas->SetNewAnimUV(17u, 5u, 0u, 17u, 0u, 5u);
	//	Atlas->AddAnim2D(DEFAULT_RES::ANIM2D::CORSAIRMOVE, 0u, 17u, 0u, 5u, 0.3f, eANIM_TYPE::DIRECTIONAL_COL_HALF_FLIP);

	//	AddRes<CAnim2DAtlas>(DEFAULT_RES::TEXTURE::CORSAIR_ATLAS, Atlas);
	//}

	//{
	//	Ptr<CAnim2DAtlas> Atlas = new CAnim2DAtlas;
	//	Atlas->SetAtlasTexture(FindRes<CTexture>(DEFAULT_RES::TEXTURE::MARINE_ATLAS));

	//	Atlas->SetNewAnimUV_SC_Redundant(14u, 0u, 14u);
	//	Atlas->AddAnim2D_SC_Redundant(DEFAULT_RES::ANIM2D::MARINE_IDLE, 0u, 1u, 1.f);
	//	Atlas->AddAnim2D_SC_Redundant(DEFAULT_RES::ANIM2D::MARINE_ATTACKSTART, 1u, 2u, 0.5f);

	//	Atlas->AddAnim2D_SC_Redundant(DEFAULT_RES::ANIM2D::MARINE_MOVE, 4u, 9u, 1.f);

	//	vector<UINT> row = { 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 3u, 2u, 2u, 2u, 2u, 2u };
	//	Atlas->AddAnim2D_vecRowIndex(DEFAULT_RES::ANIM2D::MARINE_ATTACK, row, 0.6f);

	//	row.clear();
	//	
	//	row = { 13u, 27u, 41u, 55u, 69u, 84u, 98u, 112u };
	//	Atlas->AddAnim2D(DEFAULT_RES::ANIM2D::MARINE_DEATH, row, 1.f);
	//	

	//	AddRes<CAnim2DAtlas>(DEFAULT_RES::TEXTURE::MARINE_ATLAS, Atlas);
	//}
}


