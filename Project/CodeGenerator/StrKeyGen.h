#pragma once

#include "pch.h"
//_PathFromContent : Content 폴더로부터의 경로
//ex) 텍스처 폴더를 순회돌면서 파일명을 추가하고 싶을 경우 "Texture"만 넣어주면 됨.
//_HeaderFileName : 헤더 파일의 이름만 작성
//_vecExtension : 가져올 확장자 종류

class CDirTree;
class StrKeyGen
{
public:
	StrKeyGen() = default;
	~StrKeyGen() = default;

public:
	void ReadFiles(const std::filesystem::path& _PathFromContent, const vector<string>& _vecExtension);

	static void CreateStrKey_EnumArray(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

	static void CreateStrKey_Hierarchy(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);

	static void CreateStrKey(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension);
};

