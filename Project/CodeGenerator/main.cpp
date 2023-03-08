// CodeGenerator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"

#include "CodeGenFunc.h"

//한글 경로 지원 안함.(어차피 여기서 지원해도 Unity Build에서 지원 안됨)
//필요 argument : 2개
//argv[1] = 처리해야할 작업의 종류(ex. "Script")
//argv[2] = 작업 경로(ex. "...(경로)...\\repos\\DX_MyFramework\\Project\\Script")
int main(int argc, char* argv[])
{
    //arguments가 만족되지 않으면 리턴
    if (2 > argc)
    {
        if (0 == argc)
            MessageBoxA(nullptr, "2 Parameters Missing", NULL, MB_OK);
        else if (1 == argc)
            MessageBoxA(nullptr, "1 Parameters Missing", NULL, MB_OK);

        return 0;
    }
        


    //프로젝트 주소를 받아 온다.
    string Path = argv[2];

    //argument를 비교.
    if (0 == strcmp("Script", argv[1]))
        CreateScriptCode(Path);
    else if (0 == strcmp("Shader", argv[1]))
        CreateShaderCode(Path);


    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
