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
        outPath /= "strKey_Texture.h";
        DirTree.CreateStrKeyHeader(outPath, "Texture", false);
    }



    //Generate Compute Shader Key
    {
        std::regex reg(define_Preset::Regex::AllShader::A);

        cDirTree DirTree;
        DirTree.SearchRecursive(define_Preset::Path::HLSL_Proj::A, reg);

        stdfs::path strKeyPath = define_Preset::Path::ScriptProj::A;
        strKeyPath /= "strKey_Shader.h";
        DirTree.CreateShaderStrKey(strKeyPath);
        //DirTree.CreateStrKeyHeader(DirPath / DIRECTORY_NAME, "Shader", true);

        //일단 미사용
        //DirTree.CreateCShaderCode(DirPath / define_Preset::Path::UserClassInit_CS::A);
        
    }

    //Generate Componets
    {
        std::regex regexCom(R"(cCom_\w+\.h|cScript_\w+\.h)");

        cDirTree DirTree;
        stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
        
        DirTree.SearchRecursive(DirPath, regexCom);

        DirTree.CreateStrKeyHeader(DirPath / "strKey_Component.h", "Com", true);
        
        tAddBaseClassDesc Desc = {};
        Desc.BaseType = "IComponent";
        Desc.IncludePCH = R"(#include "pch.h")";
        Desc.IncludeMasterHeader = R"(#include "UserClassInitializer.h")";
        Desc.IncludeStrKeyHeaderName = R"(#include "strKey_Component.h")";
        Desc.IncludeManagerHeader = "#include <Engine/cComMgr.h>";
        
        Desc.Constructor_T_MacroDefine = R"(cComMgr::GetInst()->AddComConstructor<T>(strKey_Com::##T))";
        Desc.UserClassMgr_InitFuncName = "InitCom()";
        Desc._FilePath = DirPath / "UserClassInitializer_Component.cpp";

        DirTree.CreateComMgrInitCode(Desc);
    }

    ////Generate Script Key and Code
    //{
    //    std::regex regexScript(define_Preset::Regex::ScriptRegex::A);

    //    cDirTree DirTree;
    //    stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
    //    DirTree.SearchRecursive(DirPath, regexScript);

    //    DirTree.CreateStrKeyHeader(DirPath / "strKey_Script.h", "Script", true);

    //    tAddBaseClassDesc Desc = {};
    //    Desc.BaseType = "IScript";
    //    Desc.IncludeStrKeyHeaderName = "strKey_Script.h";
    //    Desc.Constructor_T_MacroDefine = R"(cUserClassMgr::GetInst()->AddScriptConstructor(strKey_Script::T, Constructor_T<IScript, T>))";
    //    Desc.UserClassMgr_InitFuncName = "InitScript()";
    //    Desc._FilePath = DirPath / "UserClassInitializer_Script.cpp";

    //    DirTree.CreateComMgrInitCode(Desc);
    //}

    ////Generate User Components
    //{
    //    std::regex regexScript(define_Preset::Regex::ComponentRegex::A);

    //    cDirTree DirTree;
    //    stdfs::path DirPath = define_Preset::Path::ScriptProj::A;
    //    DirTree.SearchRecursive(DirPath, regexScript);

    //    DirTree.CreateStrKeyHeader(DirPath / "strKey_Component.h", "Component", true);

    //    tAddBaseClassDesc Desc = {};
    //    Desc.BaseType = "IComponent";
    //    Desc.IncludeStrKeyHeaderName = "strKey_Component.h";
    //    Desc.Constructor_T_MacroDefine = R"(cUserClassMgr::GetInst()->AddComponentConstructor(strKey_Script::T, Constructor_T<IComponent, T>))";
    //    
    //    Desc.UserClassMgr_InitFuncName = "InitComponent()";
    //    Desc._FilePath = DirPath / "UserClassInitializer_Component.cpp";

    //    DirTree.CreateComMgrInitCode(Desc);
    //}

    return 0;
}
