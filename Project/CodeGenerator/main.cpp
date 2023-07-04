// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CodeGenFunc.h"

#include <Engine/strKey_Default.h>
#include "MacroFunc.h"

#include "CDirTree.h"


int m_iIndentation = 0;

//한글 경로 지원 안함.(어차피 여기서 지원해도 Unity Build에서 지원 안됨)
//argv[1] = 처리해야할 작업의 종류(ex. Script)

//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{
    //Generate Texture Key
    {
        vector<stdfs::path> vecExtension;
        for (size_t i = 0; i < (size_t)RES_INFO::TEXTURE::Ext::idx::END; ++i)
        {
            vecExtension.push_back(RES_INFO::TEXTURE::Ext::arr[i]);
        }
        vector<stdfs::path> vecExcludeDir;

        CDirTree TexTree;
        stdfs::path TexPath = define_Preset::Path::Content::A;
        TexPath /= RES_INFO::TEXTURE::DirName;
        TexTree.SearchRecursive(TexPath, vecExtension, vecExcludeDir);

        TexTree.CreateStrKey<char>("D:\\Users\\ekdrn\\Desktop", "TextureKeys.h");

        //CreateStrKey(RES_INFO::TEXTURE::DirName, define_Preset::Path::strKey_Texture, vecExtension);
    }

    //Generate Shader Key
    {
        vector<stdfs::path> vecExtension;
        for (size_t i = 0; i < (size_t)RES_INFO::TEXTURE::Ext::idx::END; ++i)
        {
            vecExtension.push_back(RES_INFO::SHADER::Ext_ShaderCode);
        }
        vector<stdfs::path> vecExcludeDir;
        //vecExcludeDir.push_back(RES_INFO::SHADER::ByteCodeDir);

        CDirTree TexTree;
        stdfs::path TexPath = define_Preset::Path::Content::A;
        TexPath /= RES_INFO::SHADER::DirNameRoot;
        TexPath /= RES_INFO::SHADER::ByteCodeDir;
        TexTree.SearchRecursive(TexPath, vecExtension, vecExcludeDir);

        TexTree.CreateShaderClassCode<char>("D:\\Users\\ekdrn\\Desktop", "ShaderTest.h");

        //TexTree.CreateShaderClassCode()
        

        //CreateStrKey(RES_INFO::TEXTURE::DirName, define_Preset::Path::strKey_Texture, vecExtension);

    }

    //Generate Prefab Key
    //{
    //    vector<string> vecExt;
    //    vecExt.push_back(RES_INFO::PREFAB::Ext);
    //    CreateStrKey(RES_INFO::PREFAB::DirName, define_Preset::Path::strKey_Prefab, vecExt);
    //}
    //

    ////argument가 만족되지 않으면 디버그 모드로 작동
    //if (1 == argc)
    //{
    //    MessageBoxA(nullptr, "No Parameters sent.\nWork in Debug Mode.", "Notification", MB_OK);



    //    //MessageBoxA(nullptr, argv[0], "Test", MB_OK);
    //    //CreateShaderCode();
    //    return 0;
    //}


    //작업 타입을 비교 후 대응되는 함수를 호출한다.
    //if (0 == strcmp("Script", argv[1]))
    //{
    //    //MessageBoxA(nullptr, "Generating Script and Shader code.", "Noti", MB_OK);
    //    CreateScriptCode();
    //    CreateShaderCode();


    //    vector<string> vecExtension;
    //    for (size_t i = 0; i < (int)RES_INFO::TEXTURE::eTEX_TYPE::END; ++i)
    //    {
    //        vecExtension.push_back(string(RES_INFO::TEXTURE::ExtensionArr[i]));
    //    }
    //    //CreateStrKey(RES_INFO::TEXTURE::DirName, "strKey_Texture.h", vecExtension);
    //}
    //    

    return 0;
}
