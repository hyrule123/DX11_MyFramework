#include "pch.h"
#include "CShaderLoader.h"

#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CResMgr.h"

#include "CPathMgr.h"

#include "jsoncpp.h"

CShaderLoader::CShaderLoader()
	: m_jsonShaderInfo()
	, m_bValueModified()
{
}

CShaderLoader::~CShaderLoader()
{
	if (true == m_bValueModified)
	{
		wstring JsonFilePath = RELATIVE_PATH::SHADER::W;
		//JsonFilePath += JSON_SHADERINFO::strJSONFilenameW;

		std::ofstream fpJson(JsonFilePath);

		if (true == fpJson.is_open())
		{
			fpJson << *m_jsonShaderInfo;
			fpJson.close();
		}
		else
		{
			ERROR_MESSAGE(Unable to create file.\n Aborting save.);
		}
	}
}

bool CShaderLoader::LoadJsonFile()
{
	wstring JsonFilePath = RELATIVE_PATH::SHADER::W;
	//JsonFilePath += JSON_SHADERINFO::strJSONFilenameW;
	std::ifstream fpJson(JsonFilePath);

	if (false == fpJson.is_open())
		return false;

	SAFE_DELETE(m_jsonShaderInfo);

	m_jsonShaderInfo = new Json::Value;

	fpJson >> (*m_jsonShaderInfo);
	fpJson.close();

	return true;
}

bool CShaderLoader::LoadAllShaders()
{
	const Json::Value& SInfo = *m_jsonShaderInfo;

	//TODO : 파일 순회 돌면서 쉐이더 로드하기

	return false;
}

const Json::Value* CShaderLoader::GetShaderInfo(const string& _strShaderKey)
{
	if (false == m_jsonShaderInfo->isMember(_strShaderKey))
		return nullptr;

		
	return &((*m_jsonShaderInfo)[_strShaderKey]);
}

bool CShaderLoader::CreateDefaultGraphicsShader(const wstring& _wstrShaderBasePath, const Json::Value& _ShaderInfo)
{
	// ============
	// Debug Shader
	// Topology: LineStrip
	// Rasterizer: No Culling
	// Blend State: Default
	// Shader Domain: Opaque
	// ============
	auto iter = _ShaderInfo.begin();
	const auto& iterEnd = _ShaderInfo.end();
	for (iter; iter != iterEnd; ++iter)
	{
		Ptr<CGraphicsShader> GS = new CGraphicsShader;

		//GS->Load()

		//int ePipelineFlag = (*iter)[JSON_SHADERINFO::PipelineFlag].asInt();

		//int numShader = 0;
		//for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
		//{
		//	//플래그값에 일치하는 쉐이더가 있을경우 이름을 만들어준다.q
		//	if (ePipelineFlag & (1 << i))
		//	{
		//		++numShader;
		//		std::filesystem::path shaderName = RELATIVE_PATH::SHADER::U8;

		//		shaderName += u8"S_";
		//		shaderName += std::to_string(numShader);
		//		switch ((eSHADER_TYPE)i)
		//		{
		//		case eSHADER_TYPE::__VERTEX:
		//			shaderName += JSON_SHADERINFO::VertexShaderName;
		//			break;
		//		case eSHADER_TYPE::__HULL:
		//			shaderName += JSON_SHADERINFO::HullShaderName;
		//			break;
		//		case eSHADER_TYPE::__DOMAIN:
		//			shaderName += JSON_SHADERINFO::DomainShaderName;
		//			break;
		//		case eSHADER_TYPE::__GEOMETRY:
		//			shaderName += JSON_SHADERINFO::GeometryShaderName;
		//			break;
		//		case eSHADER_TYPE::__PIXEL:
		//			shaderName += JSON_SHADERINFO::PixelShaderName;
		//			break;
		//		case eSHADER_TYPE::END:
		//			break;
		//		default:
		//			break;
		//		}
		//		shaderName += (*iter)[JSON_SHADERINFO::ShaderName].asString();
		//		shaderName += JSON_SHADERINFO::ShaderExtension;

		//		GS->Load(shaderName);
		//	}
		//}


		//GS->SetKey(iter.key().asString());
		//CResMgr::GetInst()->AddRes<CGraphicsShader>(GS->GetKey(), GS);

	}


	//TODO : 여기 파일시스템 프로젝트로 다 긁어와서 알아서 컴파일하도록 하는 기능 추가하기
	{



		//Ptr<CGraphicsShader> pShader = new CGraphicsShader;

		//if (true == fpVS.is_open())
		//{
		//	std::streampos fileSize = fpVS.tellg();

		//	char* VS = new char[fileSize];
		//	memset(VS, 0, fileSize);

		//	fpVS.seekg(0, std::ios::beg);
		//	fpVS.read(VS, fileSize);

		//	pShader->CreateShader((void*)VS, fileSize, eSHADER_TYPE::__VERTEX);


		//	int a = 0;
		//}
	}
	//{
	//	pShader->SetKey(DEFAULT_RES::SHADER::DEBUG);
	//	pShader->CreateShader((void*)g_VS_Debug, sizeof(g_VS_Debug), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_Debug, sizeof(g_PS_Debug), eSHADER_TYPE::__PIXEL);
	//	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_NONE);
	//	pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_TEST_NO_WRITE);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//}

	//// ===========
	//// Test Shader
	//// =========== 
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::TEST);
	//	pShader->CreateShader((void*)g_VS_test, sizeof(g_VS_test), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_test, sizeof(g_PS_test), eSHADER_TYPE::__PIXEL);
	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	pShader->SetBlendState(eBLENDSTATE_TYPE::DEFAULT);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	AddRes(pShader->GetKey(), pShader);
	//}

	//// ============
	//// std2D Shader
	//// ============
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::STD2D);
	//	pShader->CreateShader((void*)g_VS_std2D, sizeof(g_VS_std2D), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_std2D, sizeof(g_PS_std2D), eSHADER_TYPE::__PIXEL);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	AddRes(pShader->GetKey(), pShader);
	//}


	//// ==================
	//// std2D_Light Shader
	//// ==================
	//// 광원을 처리할 수 있는 2D 쉐이더
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::STD2D_LIGHT);
	//	pShader->CreateShader((void*)g_VS_std2D_Light, sizeof(g_VS_std2D_Light), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_std2D_Light, sizeof(g_PS_std2D_Light), eSHADER_TYPE::__PIXEL);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_OPAQUE);
	//	AddRes(pShader->GetKey(), pShader);
	//}



	//// ===============================
	//// Tilemap_Atlas
	//// RS_TYPE : CULL_BACK
	//// DS_TYPE : LESS(Default)
	//// BS_TYPE : MASK

	//// Parameter
	//// g_CBuffer_Mtrl_Scalar.INT_0 : Tile X Count
	//// g_CBuffer_Mtrl_Scalar.int_1 : Tile Y Count
	//// g_tex_0 : Tile Atlas Texture
	////===============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::TILEMAP_ATLAS);
	//	pShader->CreateShader((void*)g_VS_Tilemap_Atlas, sizeof(g_VS_Tilemap_Atlas), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_Tilemap_Atlas, sizeof(g_PS_Tilemap_Atlas), eSHADER_TYPE::__PIXEL);
	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
	//	AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//}

	//// ===============================
	//// Tilemap_Atlas
	//// RS_TYPE : CULL_BACK
	//// DS_TYPE : LESS(Default)
	//// BS_TYPE : MASK

	//// Parameter
	//// g_tex_0 : Tile Atlas Texture
	////===============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::TILEMAP_COMPLETE);
	//	pShader->CreateShader((void*)g_VS_Tilemap_Complete, sizeof(g_VS_Tilemap_Complete), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_PS_Tilemap_Complete, sizeof(g_PS_Tilemap_Complete), eSHADER_TYPE::__PIXEL);
	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	pShader->SetBlendState(eBLENDSTATE_TYPE::MASK);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_MASK);
	//	AddRes<CGraphicsShader>(pShader->GetKey(), pShader);
	//}




	//// ============================
	//// ParticleRender
	//// 
	//// RS_TYPE : CULL_NONE
	//// DS_TYPE : NO_WRITE
	//// BS_TYPE : ALPHA_BLEND

	//// Parameter
	//// g_CBuffer_Mtrl_Scalar.INT_0 : Particle Index
	//// 
	//// Domain : TRANSPARENT
	//// ============================
	//{
	//	Ptr<CGraphicsShader> pShader = new CGraphicsShader;
	//	pShader->SetKey(DEFAULT_RES::SHADER::PARTICLE_RENDER);
	//
	//	pShader->CreateShader((void*)g_VS_Particle, sizeof(g_VS_Particle), eSHADER_TYPE::__VERTEX);
	//	pShader->CreateShader((void*)g_GS_Particle, sizeof(g_GS_Particle), eSHADER_TYPE::__GEOMETRY);
	//	pShader->CreateShader((void*)g_PS_Particle, sizeof(g_PS_Particle), eSHADER_TYPE::__PIXEL);

	//	pShader->SetRasterizerState(eRASTERIZER_TYPE::CULL_BACK);
	//	pShader->SetDepthStencilState(eDEPTHSTENCIL_TYPE::NO_WRITE);
	//	pShader->SetBlendState(eBLENDSTATE_TYPE::ALPHA_BLEND);
	//	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//	pShader->SetShaderDomain(eSHADER_DOMAIN::_TRANSPARENT); //알파 블렌딩을 사용하므로


	//	AddRes(pShader->GetKey(), pShader);
	//}


return false;
}

bool CShaderLoader::CreateDefaultComputeShader(const wstring& _wstrShaderBasePath, const Json::Value& _ShaderInfo)
{
	//Ptr<CComputeShader> pInitCS = new CCS_Initialize;
	//pInitCS->CreateShader((void*)g_CS_HLSL_Init, sizeof(g_CS_HLSL_Init));
	//pInitCS->SetKey(DEFAULT_RES::SHADER::COMPUTE::INIT_SETTING);
	//AddRes(pInitCS->GetKey(), pInitCS);
	//pInitCS->Execute();

	//Ptr<CComputeShader> pShader = new CCS_SetColor(32u, 32u, 1u);
	//pShader->CreateShader((void*)g_CS_SetColor, sizeof(g_CS_SetColor));
	//pShader->SetKey(DEFAULT_RES::SHADER::COMPUTE::SET_COLOR);
	//AddRes(pShader->GetKey(), pShader);
	//pShader = nullptr;

	////기본 파티클 쉐이더
	//pShader = new CCS_ParticleUpdate(128u, 1u, 1u);
	//pShader->CreateShader((void*)g_CS_Particle_Basic, sizeof(g_CS_Particle_Basic));
	//pShader->SetKey(DEFAULT_RES::SHADER::COMPUTE::PARTICLE_UPDATE_BASIC);
	//AddRes(pShader->GetKey(), pShader);
	//pShader = nullptr;

	////비 효과 파티클 쉐이더
	//pShader = new CCS_ParticleUpdate(128u, 1u, 1u);
	//pShader->CreateShader((void*)g_CS_Particle_RainDrop, sizeof(g_CS_Particle_RainDrop));
	//pShader->SetKey(DEFAULT_RES::SHADER::COMPUTE::PARTICLE_UPDATE_RAINDROP);
	//AddRes(pShader->GetKey(), pShader);
	//pShader = nullptr;


	//pShader = new CCS_SCMapLoader;
	//pShader->CreateShader((void*)g_CS_SCMapLoader, sizeof(g_CS_SCMapLoader));
	//pShader->SetKey(DEFAULT_RES::SHADER::COMPUTE::SC_MAP_LOADER);
	//AddRes(pShader->GetKey(), pShader);

	return false;
}
