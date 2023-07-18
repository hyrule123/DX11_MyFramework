// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CodeGenFunc.h"

#include <Engine/strKey_Default.h>
#include <Engine/func.h>

#include "cDirTree.h"


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

        cDirTree DirTree;
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
        std::regex reg(define_Preset::Regex::AllShader::A);

        cDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, reg);

        
        
        DirTree.CreateShaderStrKey(DirPath / "strKey_Shader.h");
        //DirTree.CreateStrKeyHeader(DirPath / DIRECTORY_NAME, "Shader", true);

        //일단 미사용
        //DirTree.CreateCShaderCode(DirPath / define_Preset::Path::UserClassInit_CS::A);
        
    }

    //Generate Graphics Shader Key
    //{
    //    //std::regex regexGS(define_Preset::Regex::Shader::A);


    //    cDirTree DirTree;
    //    stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
    //    DirTree.SearchRecursive(DirPath, regexGS);

    //    DirTree.CreateShaderStrKey(DirPath / define_Preset::Path::strKey_GShader::A);
    //}

    //Generate Script Key and Code
    {
        std::regex regexScript(R"(cScript_.+\.h)");

        cDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        DirTree.SearchRecursive(DirPath, regexScript);

        DirTree.CreateStrKeyHeader(DirPath / define_Preset::Path::strKey_Script::A, "Script", true);

        tAddBaseClassDesc Desc = {};
        Desc.IncludeBaseHeaderName = "IScript.h";
        Desc.IncludeStrKeyHeaderName = "strKey_Script.h";
        Desc.Constructor_T_MacroDefine = define_Preset::Keyword::define_T_Constructor_Script::A;
        Desc.UserClassMgr_InitFuncName = "InitScript()";
        //Desc.Constructor_T_FuncName = 
        Desc._FilePath = DirPath / define_Preset::Path::UserClassInit_Script::A;

        DirTree.CreateUserClassInitCode(Desc);
        //DirTree.CreateScriptCPP(DirPath / define_Preset::Path::UserClassInit_Script::A);
    }

    return 0;
}
