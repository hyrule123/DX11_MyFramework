#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"


#include "shaders.h"

#include "CCS_SetColor.h"
#include "CCS_ParticleUpdate.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
}



void CResMgr::init()
{
	CreateResClassTypeIndex();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();

	LoadDefaultTexture();
}

void CResMgr::CreateResClassTypeIndex()
{
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESH));
	//m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), eRES_TYPE::MESHDATA));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), eRES_TYPE::MATERIAL));
	m_umapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), eRES_TYPE::TEXTURE));
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
	// =============
	// RectMesh 생성
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.0f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.0f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.0f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.0f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
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
	AddRes("RectMesh", pMesh);
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
	AddRes("RectMesh_Debug", pMesh);
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
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
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
	AddRes("CircleMesh", pMesh);
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
	AddRes("CircleMesh_Debug", pMesh);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	// ============
	// Debug Shader
	// Topology: LineStrip
	// Rasterizer: No Culling
	// Blend State: Default
	// Shader Domain: Opaque
	// ============
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;

		pShader->SetKey("DebugShader");
		pShader->CreateShader((void*)g_VS_Debug, sizeof(g_VS_Debug), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_Debug, sizeof(g_PS_Debug), eSHADERTYPE_PIXEL);
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
		pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_TEST_NO_WRITE);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	}

	// ===========
	// Test Shader
	// =========== 
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey("TestShader");
		pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADERTYPE_PIXEL);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
		pShader->SetBlendState(eBLENDSTATE_TYPE::DEFAULT);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes("TestShader", pShader);
	}

	// ============
	// std2D Shader
	// ============
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey("std2DShader");
		pShader->CreateShader((void*)g_VS_std2D, sizeof(g_VS_std2D), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_std2D, sizeof(g_PS_std2D), eSHADERTYPE_PIXEL);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes(pShader->GetKey(), pShader);
	}


	// ==================
	// std2D_Light Shader
	// ==================
	// 광원을 처리할 수 있는 2D 쉐이더
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey("std2DLightShader");
		pShader->CreateShader((void*)g_VS_std2D_Light, sizeof(g_VS_std2D_Light), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_std2D_Light, sizeof(g_PS_std2D_Light), eSHADERTYPE_PIXEL);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
		AddRes(pShader->GetKey(), pShader);
	}



	// ===============================
	// Tilemap
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS(Default)
	// BS_TYPE : MASK

	// Parameter
	// g_int_0 : Tile X Count
	// g_int_1 : Tile Y Count
	// g_tex_0 : Tile Atlas Texture
	//===============================
	{
		Ptr<CGraphicsShader> pShader = new CGraphicsShader;
		pShader->SetKey("TilemapShader");
		pShader->CreateShader((void*)g_VS_Tilemap, sizeof(g_VS_Tilemap), eSHADERTYPE_VERTEX);
		pShader->CreateShader((void*)g_PS_Tilemap, sizeof(g_PS_Tilemap), eSHADERTYPE_PIXEL);
		pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
		pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
		pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	}
}

void CResMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pShader = new CCS_SetColor(32u, 32u, 1u);
	pShader->SetKey("SetColorCS");
	pShader->CreateShader((void*)g_CS_SetColor, sizeof(g_CS_SetColor));
	AddRes(pShader->GetKey(), pShader);


	pShader = new CCS_ParticleUpdate;
	pShader->SetKey("ParticleUpdate");
	pShader->CreateShader((void*)g_CS_Particle, sizeof(g_CS_Particle));
	
	AddRes(pShader->GetKey(), pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	//Debug Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetKey("DebugMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>("DebugShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Test Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial;
		pMtrl->SetKey("TestMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>("TestShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2D Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial;
		pMtrl->SetKey("std2DMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>("std2DShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2DLight Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial;
		pMtrl->SetKey("std2DLightMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>("std2DLightShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Tilemap Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetKey("TilemapMtrl");
		pMtrl->SetShader(FindRes<CGraphicsShader>("TilemapShader"));
		AddRes(pMtrl->GetKey(), pMtrl);
	}
}

void CResMgr::LoadDefaultTexture()
{
	Load<CTexture>("Fighter", L"texture/Fighter.bmp");
	Load<CTexture>("HOS", L"texture/HOS.png");

	Load<CTexture>("beheaded", L"texture/beheaded.png");
	Load<CTexture>("beheaded_n", L"texture/beheaded_n.png");

	Load<CTexture>("TileAtlas", L"texture/TILE.bmp");
	Load<CTexture>("LinkAtlas", L"texture/link.png");
}


