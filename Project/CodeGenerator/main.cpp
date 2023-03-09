// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CodeGenFunc.h"


//한글 경로 지원 안함.(어차피 여기서 지원해도 Unity Build에서 지원 안됨)
//argv[1] = 처리해야할 작업의 종류(ex. Script)

//시작 지점 = $(SolutionDir) : 상대 경로로 작업해주면 된다.
int main(int argc, char* argv[])
{
    //argument가 만족되지 않으면 디버그 모드로 작동
    if (1 == argc)
    {
        MessageBoxA(nullptr, "No Parameters sent.\nWork in Debug Mode.", "Notification", MB_OK);
        //MessageBoxA(nullptr, argv[0], "Test", MB_OK);
        CreateScriptCode();
        return 0;
    }


    //작업 타입을 비교 후 대응되는 함수를 호출한다.
    if (0 == strcmp("Script", argv[1]))
        CreateScriptCode();
    else if (0 == strcmp("Shader", argv[1]))
        CreateShaderCode();


    return 0;
}
