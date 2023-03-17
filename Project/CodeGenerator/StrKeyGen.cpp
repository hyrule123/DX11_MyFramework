#include "pch.h"

#include "CodeGenFunc.h"
#include "MacroFunc.h"



void CreateStrKey(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension)
{
	std::filesystem::path HeaderPath(PresetStr::ScriptProjPath);
	HeaderPath /= _HeaderFilename;

	std::ofstream fpStrKeyHeader(HeaderPath);
	if (true == fpStrKeyHeader.is_open())
	{

		string CodeLine = string(PresetStr::Head);
		fpStrKeyHeader << CodeLine;


		//여기부터 recursive iterator를 통해서 순회
		filesystem::recursive_directory_iterator dirIter;
		try
		{
			//현재 작업 영역 $(SolutionDir)로부터의 경로를 만들어준다.
			filesystem::path FullPath(PresetStr::ContentPath);

			//Fullpath = ~/Content/(리소스 폴더)
			//ex)Fullpath == ~/Content/Texture
			FullPath /= _PathFromContent;

			//리소스가 있는 경로로부터 재귀적으로 순회를 돌아준다.
			dirIter = filesystem::recursive_directory_iterator(FullPath);
			const filesystem::recursive_directory_iterator& dirIterEnd = filesystem::end(dirIter);

			//폴더구조가 변경되었는지 확인하기 위한 반복자
			filesystem::path prevPath("");
			int numDirPrev = 0;
			for (dirIter; dirIter != dirIterEnd; ++dirIter)
			{
				//폴더가 아닐 경우에만 진행
				if (false == dirIter->is_directory())
				{
					const filesystem::path& curPath = dirIter->path();


					//리소스 확장자가 일치하는지 확인.
					bool bMatchingExtension = false;
					for (size_t i = (size_t)0; i < _vecExtension.size(); ++i)
					{
						string Extension = curPath.filename().extension().string();
						transform(Extension.begin(), Extension.end(), Extension.begin(), ::tolower);

						if (_vecExtension[i] == Extension)
						{
							bMatchingExtension = true;
							break;
						}
					}

					if (false == bMatchingExtension)
						continue;


					int iComp = curPath.parent_path().compare(prevPath);

					//들어온 경로가 지난 경로와 다를 경우
					if (0 != iComp)
					{
						//타고들어온 폴더의 갯수를 계산한다.
						int numDirCur = 0;
						for (filesystem::path::iterator iter = curPath.begin(); iter != curPath.end(); ++iter)
						{
							++numDirCur;
						}

						//폴더를 하나 빠져나왔거나 폴더 위치만 변경되었을 경우(a/b -> a/c)
						//괄호를 닫아 준다.
						if (numDirPrev >= numDirCur)
						{
							WriteCodeA(fpStrKeyHeader, "}");
						}

						//두 조건 모두 같을 때(==) 조건도 확인해 주어야 한다.
						//같을 떄는 괄호를 닫은 다음, 다시 열어야 주어야 하기 때문.

						//폴더를 하나 더 파고 들어왔거나, 폴더 위치만 변경되었을 경우(a/b -> a/c)
						//부모 폴더명으로 네임스페이스 이름을 생성한다.
						//폴더만 바뀌었을 경우에는 괄호를 닫아 주어야 한다.
						if (numDirPrev <= numDirCur)
						{
							CodeLine = PresetStr::NameSpace;

							string UpperCase = curPath.parent_path().filename().string();
							transform(UpperCase.begin(), UpperCase.end(), UpperCase.begin(), ::toupper);

							CodeLine += UpperCase;							
							WriteCodeA(fpStrKeyHeader, CodeLine);
							WriteCodeA(fpStrKeyHeader, "{");
						}


						//prevPath를 변경.
						prevPath = curPath.parent_path();
						numDirPrev = numDirCur;
					}


					//키값을 추가한다. 이 때, 키(변수명)은 파일명으로만 해놓고,
					//실제 Value 값은 자신의 리소스 타입 이후로 해준다.
					//ex. Texture/SCUnit/Marine.png -> SCUnit/Marine.png
					CodeLine = PresetStr::ConstexprStringView;

					string UpperCase = curPath.filename().replace_extension("").string();
					transform(UpperCase.begin(), UpperCase.end(), UpperCase.begin(), ::toupper);
					CodeLine += UpperCase;
					CodeLine += PresetStr::EqualDoubleQuotation;

					const filesystem::path& ValuePath = curPath.lexically_relative(FullPath);
					CodeLine += ValuePath.string();
					CodeLine += "\";";

					WriteCodeA(fpStrKeyHeader, CodeLine);




				}

			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			MessageBoxA(nullptr, error.what(), NULL, MB_OK);
			throw(std::runtime_error("Process Terminated by error."));
		}

		//작성이 완료되었으면 파일을 저장한다.
		fpStrKeyHeader.close();
	}


	
}


