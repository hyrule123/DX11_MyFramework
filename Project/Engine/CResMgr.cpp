#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "strKey_Default.h"

#include "CCS_SetColor.h"
#include "CCS_ParticleUpdate.h"
#include "CCS_SCMapLoader.h"
#include "CCS_Initialize.h"

#include "CAnim2DAtlas.h"
#include "CGameObject.h"

#include <fstream>


CResMgr::CResMgr()
	: m_bResUpdated(true)
{
	
}

CResMgr::~CResMgr()
{
	//메모리 릭 잡기용 코드
	//for (UINT i = 0; i < (UINT)eRES_TYPE::END; ++i)
	//{
	//	auto iter = m_arrRes[i].begin();
	//	auto iterEnd = m_arrRes[i].end();
	//	while (iter != iterEnd)
	//	{
	//		OutputDebugStringA(string(iter->second->GetKey() + ": ").c_str());
	//		OutputDebugStringA(string(std::to_string(iter->second->GetRefCount()) + "\n").c_str());
	//		iter = m_arrRes[i].erase(iter);
	//	}
	//}

}



void CResMgr::init()
{
	CreateResClassTypeIndex();

	CreateDefaultMesh();
	
	CreateDefaultShader();

	CreateDefaultMaterial();

	LoadDefaultTexture();
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
	AddRes(string(strKey_RES_DEFAULT::MESH::POINT), pMesh);
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
	AddRes(string(strKey_RES_DEFAULT::MESH::RECT), pMesh);
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
	AddRes(string(strKey_RES_DEFAULT::MESH::DEBUG_RECT), pMesh);
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
	AddRes(string(strKey_RES_DEFAULT::MESH::CIRCLE), pMesh);
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
	AddRes(string(strKey_RES_DEFAULT::MESH::DEBUG_CIRCLE), pMesh);
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

	std::filesystem::path GSDir(CPathMgr::GetInst()->GetPathRel_Resource(eRES_TYPE::GRAPHICS_SHADER));

	//Shader 폴더 순회돌면서 전부 로드하는 코드
	std::filesystem::directory_iterator GSDirIter;
	try
	{
		GSDirIter = std::filesystem::directory_iterator(GSDir);
	}
	catch (const std::filesystem::filesystem_error& error)
	{
		ERROR_RUNTIME(error);
		return false;
	}
	

	for (; false == GSDirIter._At_end(); ++GSDirIter)
	{
		if (false == GSDirIter->is_directory())
		{
			const string& fileName = GSDirIter->path().extension().string();

			//.json 파일일 경우 동적할당하고 로드한다.
			if (string::npos != fileName.find(RES_INFO::SHADER::Ext_ShaderSetting))
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
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::DEBUG);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	

	//	//pShader->SetKey(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::DEBUG));
	//	//pShader->CreateShader((void*)g_VS_Debug, sizeof(g_VS_Debug), eSHADER_TYPE::__VERTEX);
	//	//pShader->CreateShader((void*)g_PS_Debug, sizeof(g_PS_Debug), eSHADER_TYPE::__PIXEL);
	//	//pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//	//pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
	//	//pShader->SetDepthStencilState(eDEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);
	//	//pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	
	//}



	////// ===========
	////// Test Shader
	////// =========== 
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TEST);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	
	//	//pShader->SetKey(strKey_RES_DEFAULT::SHADER::TEST);
	//	//pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADER_TYPE::__VERTEX);
	//	//pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADER_TYPE::__PIXEL);
	//	//pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	//pShader->SetBlendState(eBLEND_STATE_TYPE::DEFAULT);
	//	//pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	//AddRes(pShader->GetKey(), pShader);
	//}

	////// ============
	////// std2D Shader
	////// ============
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::STD2D);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(strKey_RES_DEFAULT::SHADER::STD2D);
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
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::STD2DLIGHT);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(strKey_RES_DEFAULT::SHADER::STD2D_LIGHT);
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
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TILEMAPATLAS);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(strKey_RES_DEFAULT::SHADER::TILEMAP_ATLAS);
	////	pShader->CreateShader((void*)g_VS_Tilemap_Atlas, sizeof(g_VS_Tilemap_Atlas), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_PS_Tilemap_Atlas, sizeof(g_PS_Tilemap_Atlas), eSHADER_TYPE::__PIXEL);
	////	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	////	pShader->SetBlendState(eBLEND_STATE_TYPE::MASK);
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
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TILEMAPCOMPLETE);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	//	pShader->SetKey(strKey_RES_DEFAULT::SHADER::TILEMAP_SC);
	//	//	pShader->CreateShader((void*)g_VS_Tilemap_Complete, sizeof(g_VS_Tilemap_Complete), eSHADER_TYPE::__VERTEX);
	//	//	pShader->CreateShader((void*)g_PS_Tilemap_Complete, sizeof(g_PS_Tilemap_Complete), eSHADER_TYPE::__PIXEL);
	//	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	//	pShader->SetBlendState(eBLEND_STATE_TYPE::MASK);
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
	//	std::filesystem::path ShaderPath = GSDir / string(strKey_RES_DEFAULT::SHADER::GRAPHICS::PARTICLE);
	//	if (true == pShader->Load(ShaderPath))
	//		AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	////	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	////	pShader->SetKey(strKey_RES_DEFAULT::SHADER::PARTICLE_RENDER);
	////
	////	pShader->CreateShader((void*)g_VS_Particle, sizeof(g_VS_Particle), eSHADER_TYPE::__VERTEX);
	////	pShader->CreateShader((void*)g_GS_Particle, sizeof(g_GS_Particle), eSHADER_TYPE::__GEOMETRY);
	////	pShader->CreateShader((void*)g_PS_Particle, sizeof(g_PS_Particle), eSHADER_TYPE::__PIXEL);

	////	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	////	pShader->SetDepthStencilState(eDEPTH_STENCIL_TYPE::NO_WRITE);
	////	pShader->SetBlendState(eBLEND_STATE_TYPE::ALPHA_BLEND);
	////	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	////	pShader->SetShaderDomain(eSHADER_DOMAIN::_TRANSPARENT); //알파 블렌딩을 사용하므로


	////	AddRes(pShader->GetKey(), pShader);
	//}
}







bool CResMgr::CreateDefaultComputeShader()
{
	//컴퓨트쉐이더는 클래스에 종속적이므로 수동으로 로드해줘야 한다.
	try
	{
		Ptr<CComputeShader> pCS = new CCS_Initialize;

		//Initialize 컴퓨트쉐이더는 초기화와 동시에 한번 실행시켜서 초기 데이터를 등록한다.
		AddRes(pCS->GetKey(), pCS);
		pCS->Execute();
	}
	catch (const std::runtime_error& error)
	{
		ERROR_RUNTIME(error);
	}

	try
	{
		Ptr<CComputeShader>pCS = new CCS_SetColor;
		AddRes(pCS->GetKey(), pCS);
	}
	catch (const std::runtime_error& error)
	{
		ERROR_RUNTIME(error);
	}

	//ParticleUpdate 클래스는 여러개의 파티클 쉐이더를 처리해야 하므로 생성자로 기본 이름을 받고 있음.
	try
	{
		Ptr<CComputeShader> pCS = new CCS_ParticleUpdate(string(strKey_RES_DEFAULT::SHADER::COMPUTE::PARTICLEBASIC));
		AddRes(pCS->GetKey(), pCS);
	}
	catch (const std::runtime_error& error)
	{
		ERROR_RUNTIME(error);
	}

	try
	{
		Ptr<CComputeShader> pCS = new CCS_ParticleUpdate(string(strKey_RES_DEFAULT::SHADER::COMPUTE::PARTICLERAINDROP));
		AddRes(pCS->GetKey(), pCS);
	}
	catch (const std::runtime_error& error)
	{
		ERROR_RUNTIME(error);
	}

	try
	{
		Ptr<CComputeShader> pCS = new CCS_SCMapLoader;
		AddRes(pCS->GetKey(), pCS);
	}
	catch (const std::runtime_error& error)
	{
		ERROR_RUNTIME(error);
	}

	return true;
}



void CResMgr::CreateDefaultMaterial()
{
	//Debug Material
	{
		//Rectangle
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::DEBUG)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::DEBUG_RECT));
		AddRes(pMtrl->GetKey(), pMtrl);

		//Circle
		pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::DEBUG)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::DEBUG_CIRCLE));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Test Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TEST)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::TEST));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2D Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::STD2D)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::STD2D));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// std2DLight Material
	{
		Ptr<CMaterial> pMtrl = nullptr;
		pMtrl = new CMaterial();
		//pMtrl->SetInstancedRender(true);
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::STD2DLIGHT)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::STD2D_LIGHT));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	// Tilemap Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TILEMAPATLAS)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::TILEMAP_ATLAS));
		AddRes(pMtrl->GetKey(), pMtrl);
	}
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(string(strKey_RES_DEFAULT::SHADER::GRAPHICS::TILEMAPCOMPLETE)));

		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::TILEMAP_SC));
		AddRes(pMtrl->GetKey(), pMtrl);
	}

	//Particle Material
	{
		Ptr<CMaterial> pMtrl = new CMaterial();
		pMtrl->SetShader(FindRes<CGraphicsShader>(strKey_RES_DEFAULT::SHADER::GRAPHICS::PARTICLE));
		pMtrl->SetKey(string(strKey_RES_DEFAULT::MATERIAL::PARTICLE_RENDER));
		AddRes(pMtrl->GetKey(), pMtrl);
	}
}

void CResMgr::LoadDefaultTexture()
{
	Load<CTexture>(strKey_RES_DEFAULT::TEXTURE::DEFAULT);
	Load<CTexture>(strKey_RES_DEFAULT::TEXTURE::NOISE_1);
}


Ptr<CRes> CResMgr::Load(eRES_TYPE _eResType, const std::filesystem::path& _fileName)
{
	Ptr<CRes> pRes;

	switch (_eResType)
	{
	case eRES_TYPE::MESH:
		pRes = new CMesh;
		break;
	case eRES_TYPE::MESHDATA:
		pRes = new CMesh;
		break;
	case eRES_TYPE::MATERIAL:
		pRes = new CMaterial;
		break;
	case eRES_TYPE::TEXTURE:
		pRes = new CTexture;
		break;
	case eRES_TYPE::ANIM2D_ATLAS:
		pRes = new CAnim2DAtlas;
		break;
	case eRES_TYPE::SOUND:
		//아직 클래스가 없음
		break;
	case eRES_TYPE::PREFAB:
		//TODO : 프리팹 저장 기능 나중에 구현해야 됨
		pRes = new CPrefab;
		break;
	case eRES_TYPE::GRAPHICS_SHADER:
		pRes = new CGraphicsShader;
		break;
	case eRES_TYPE::COMPUTE_SHADER:
		//TODO : 여기 마저 채우기
		//pRes = new CComputeShader;
		break;

	default:
		ERROR_MESSAGE("Unknown Resource Type!");
		break;
	}

	if (false == pRes->Load(_fileName))
		pRes = nullptr;

	return pRes;
}

