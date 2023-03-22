#pragma once

#include "global.h"

#include "CSingleton.h"

#include "strKeyDefault.h"


namespace DIRECTORY_NAME
{
	//인덱스는 eRES_TYPE를 사용
	constexpr const std::string_view RES_ARR[(int)eRES_TYPE::END] =
	{
"MeshData",
"MeshData",
"Material",
"Texture",
"Anim2D",
"SCMap",
"Sound",
"Prefab",
"Shader/Graphics",
"Shader/Compute"
	};

	constexpr std::string_view RELATIVE_BEGIN = ".";
	constexpr std::string_view CONTENT = "Content";
	constexpr std::string_view MESHDATA = RES_ARR[(int)eRES_TYPE::MESHDATA];
	constexpr std::string_view MATERIAL = RES_ARR[(int)eRES_TYPE::MATERIAL];
	constexpr std::string_view TEXTURE = RES_ARR[(int)eRES_TYPE::TEXTURE];
	constexpr std::string_view ANIM2D = RES_ARR[(int)eRES_TYPE::ANIM2D_ATLAS];
	constexpr std::string_view SCMAP = RES_ARR[(int)eRES_TYPE::SCMAP];
	constexpr std::string_view SOUND = RES_ARR[(int)eRES_TYPE::SOUND];
	constexpr std::string_view PREFAB = RES_ARR[(int)eRES_TYPE::PREFAB];

	constexpr std::string_view SHADER_ROOT = "Shader";
	constexpr std::string_view SHADER_GRAPHICS = RES_ARR[(int)eRES_TYPE::GRAPHICS_SHADER];
	constexpr std::string_view SHADER_COMPUTE = RES_ARR[(int)eRES_TYPE::COMPUTE_SHADER];
}


class CPathMgr
	: public CSingleton<CPathMgr>
{	
	SINGLETON(CPathMgr);
private:
	std::filesystem::path m_PathAbsolute_Content;
	std::filesystem::path m_PathRelative_Content;
	std::filesystem::path m_PathRelative_Resource[(int)eRES_TYPE::END];

	

public:
	void init();
	
public:
	const std::filesystem::path& GetPathAbs_Content() const { return m_PathAbsolute_Content; }
	const std::filesystem::path& GetPathRel_Content() const { return m_PathRelative_Content; }
	const std::filesystem::path& GetPathRel_Resource(eRES_TYPE _eResType) const { return m_PathRelative_Resource[(int)_eResType]; }


};

