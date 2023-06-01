#pragma once

#include <Engine/define.h>
#include <Engine/strKey_Default.h>





void CreateScriptCode();

//쉐이더는 좀 특수한 경우(json 파일을 통해서 쉐이더 코드파일 여러개를 로드)이므로 따로 분리하였음.
void CreateShaderCode();

//_PathFromContent : Content 폴더로부터의 경로
//ex) 텍스처 폴더를 순회돌면서 파일명을 추가하고 싶을 경우 "Texture"만 넣어주면 됨.
//_HeaderFileName : 헤더 파일의 이름만 작성
//_vecExtension : 가져올 확장자 종류
void CreateStrKey_EnumArray(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

void CreateStrKey_Hierarchy(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

void CreateStrKey(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

