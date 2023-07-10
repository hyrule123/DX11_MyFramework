// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CodeGenFunc.h"

#include <Engine/strKey_Default.h>
#include "MacroFunc.h"

#include "CDirTree.h"


//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{
    //Create variable name restraints regex
    define_Preset::Regex::g_VarForbiddenChars::CreateVarForbiddendRegex();


    //Generate Texture Key
    {
        std::string regbase;
        regbase += R"((.+)\.()";

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
        {
            stdfs::path DirPath = define_Preset::Path::Content::A;
            DirPath /= RES_INFO::TEXTURE::DirName;
            DirTree.SearchRecursive(DirPath, reg);
        }

        stdfs::path outPath = define_Preset::Path::ScriptProj::A;
        outPath /= define_Preset::Path::strKey_Texture::A;
        DirTree.CreateStrKeyHeader(outPath, "Texture", false);
    }

    //Generate Compute Shader Key
    {
        std::regex reg(define_Preset::Regex::CShader::A);

        CDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, reg);

        DirTree.CreateCShaderCode(DirPath / define_Preset::Path::UserClassInit_CS::A);
        DirTree.CreateStrKeyHeader(DirPath / define_Preset::Path::strKey_CShader::A, "CShader", true);
    }

    //Generate Graphics Shader Key
    {
        std::regex regexGS(define_Preset::Regex::GShader::A);


        CDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, regexGS);

        DirTree.CreateGShaderStrKey(DirPath / define_Preset::Path::strKey_GShader::A);
    }

    //Generate Script Key and Code
    {
        std::regex regexScript(R"(CScript_.+\.h)");

        CDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, regexScript);

        DirTree.CreateStrKeyHeader(DirPath / define_Preset::Path::strKey_Script::A, "Script", true);
        DirTree.CreateScriptCPP(DirPath / define_Preset::Path::UserClassInit_Script::A);
    }

    return 0;
}
