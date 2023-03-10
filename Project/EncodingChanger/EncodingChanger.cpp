// EncodingChanger.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <string>
#include <vector>
#include <filesystem>
using namespace std;

enum class eEXTENSIONS
{
    H,
    CPP,
    HLSL,
    HLSLI,
    INL,
    END
};

constexpr const char* g_Extensions[(int)eEXTENSIONS::END] =
{
    ".h", ".cpp", ".hlsl", ".hlsli", "inl"
};


void DirIterationMain(const string& _strRelativePath = "\\");
void recursiveConvert(const string& _strRelativePath, vector<string> _vecPath);


int main()
{
    filesystem::directory_iterator iter("\\");
    auto iterEnd = filesystem::end(iter);
    for (iter; iter != iterEnd; ++iter)
    {
        const 
    }


    return 0;
}

void DirIterationMain(const string& _strRelativePath = "\\")
{

}

void recursiveConvert(const string& _strRelativePath, vector<string> _vecPath);
{
    


}