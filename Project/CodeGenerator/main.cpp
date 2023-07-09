// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CodeGenFunc.h"

#include <Engine/strKey_Default.h>
#include "MacroFunc.h"

#include "CDirTree.h"


std::regex define_Preset::Regex::g_regexVarForbidden;

//한글 경로 지원 안함.(어차피 여기서 지원해도 Unity Build에서 지원 안됨)
//argv[1] = 처리해야할 작업의 종류(ex. Script)

//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{
    {
        using namespace define_Preset::Regex;
        g_regexVarForbidden = MakeVarForbiddenRegex();
    }
    

    //Generate Texture Key
    {
        std::string regbase;
        regbase += R"((\w+)\.()";

        for (size_t i = 0; i < (size_t)RES_INFO::TEXTURE::Ext::idx::END; ++i)
        {
            string temp = RES_INFO::TEXTURE::Ext::arr[i];
            size_t pos = temp.find('.');
            if (std::string::npos != pos)
            {
                temp.erase(0, pos + 1);
            }
            regbase += temp;

            if (i == (size_t)RES_INFO::TEXTURE::Ext::idx::END - 1ui64)
                regbase += ")$";
            else
                regbase += "|";
        }

        std::regex reg(regbase, std::regex::icase);

        CDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::Content::A;
        DirPath /= RES_INFO::TEXTURE::DirName;
        DirTree.SearchRecursive(DirPath, reg);

        DirTree.CreateStrKeyHeader<char>(define_Preset::Path::ScriptProj::A, define_Preset::Path::strKey_Texture::A, false, "Texture");
    }

    //Generate Compute Shader Key
    {
        std::regex reg(define_Preset::Regex::CShader);

        CDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, reg);

        DirTree.CreateCShaderCode<char>(DirPath, define_Preset::Path::UserClassInit_CS::A);
        DirTree.CreateStrKeyHeader<char>(DirPath, define_Preset::Path::strKey_CShader::A, true, "CShader");
    }

    ////Generate Graphics Shader Key
    //{
    //    tDirTreeFilters Filter;
    //    Filter.Include_Ext(RES_INFO::SHADER::Ext_ShaderSourceCode);

    //    CDirTree DirTree;
    //    stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
    //    DirTree.SearchRecursive(DirPath, Filter);

    //    stdfs::path TargetPath = define_Preset::Path::ScriptProj::A;

    //    DirTree.CreateGShaderStrKey<char>(TargetPath, define_Preset::Path::strKey_GShader::A);
    //}

    ////Generate Script Key and Code
    //{
    //    tDirTreeFilters Filter;
    //    Filter.Include_Keyword(define_Preset::Keyword::ScriptPrefix::A);
    //    Filter.Include_Ext(".h");

    //    CDirTree DirTree;
    //    stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
    //    DirTree.SearchRecursive(DirPath, Filter);

    //    stdfs::path TargetPath = DirPath;

    //    DirTree.CreateStrKeyHeader<char>(TargetPath, define_Preset::Path::strKey_Script::A, true, "Script");
    //    DirTree.CreateScriptCPP<char>(TargetPath, define_Preset::Path::UserClassInit_Script::A);
    //}

    return 0;
}
