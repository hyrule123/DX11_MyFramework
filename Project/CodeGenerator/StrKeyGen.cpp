#include "pch.h"

#include "CodeGenFunc.h"
#include "MacroFunc.h"

/*
알고리즘 정리
1. 폴더 전체 재귀순회
2. 파일의 Parent 경로를 Key로, 파일명을 Value로해서 Vector 형태로 저장한다.
	Key = Parent 경로, Value = vector<string>: 파일명 저장
3. 그러면 같은 Parent Path를 가진 파일들은 동일한 Key에 저장됨
4. 순회가 끝나면 1차로 Map을 순회
	Root부터 순회 돌면서 Path의 자식관계를 비교
	아래는 상하관계를 비교하는 코드
	std::filesystem::path parentPath = "/usr/local";
	std::filesystem::path childPath = "/usr/local/bin";
	bool isChild = std::filesystem::relative(childPath, parentPath).empty();
	if (isChild) {
		std::cout << childPath << " is a child of " << parentPath << std::endl;
	} else {
		std::cout << childPath << " is not a child of " << parentPath << std::endl;
	}
*/




void CreateStrKey(const std::filesystem::path& _PathFromContent, const std::filesystem::path& _HeaderFilename, const vector<string>& _vecExtension)
{
	std::filesystem::path HeaderPath(PresetPath::ScriptProj);
	HeaderPath /= _HeaderFilename;

	std::ofstream fp(HeaderPath);
	if (true == fp.is_open())
	{

		string CodeLine = string(PresetStr::Head);
		fp << CodeLine;

		unordered_map<filesystem::path, vector<filesystem::path>> umapFile;

		//여기부터 recursive iterator를 통해서 순회
		filesystem::recursive_directory_iterator dirIter;
		try
		{
			//현재 작업 영역 $(SolutionDir)로부터의 경로를 만들어준다.
			filesystem::path FullPath(PresetPath::Content);

			//Fullpath = ~/Content/(리소스 폴더)
			//ex)Fullpath == ~/Content/Texture
			FullPath /= _PathFromContent;

			//리소스가 있는 경로로부터 재귀적으로 순회를 돌아준다.
			dirIter = filesystem::recursive_directory_iterator(FullPath);
			const filesystem::recursive_directory_iterator& dirIterEnd = filesystem::end(dirIter);

			for (dirIter; dirIter != dirIterEnd; ++dirIter)
			{
				//폴더가 아닐 경우에만 진행. Map을 통해서 전부 넣어준다.
				if (false == dirIter->is_directory())
				{
					const filesystem::path& curPath = dirIter->path();

					//불러올 리소스 확장자와 일치하는지 확인한다.
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

					//등록하고자 하는 확장자와 일치하지 않을 경우 등록 취소.
					if (false == bMatchingExtension)
						continue;

					//Fullpath에서 자신의 위치 경로를 제외한 값만 생성해준다.
					//
					const filesystem::path& ValuePath = curPath.lexically_relative(FullPath);

					umapFile[curPath.parent_path()].push_back(ValuePath);
				}
			}

			
			//순회가 끝났으면 코드를 생성해준다.
			//enum class부터 생성
			CodeLine = string(PresetStr::NameSpace);
			string UpperCase = string("RES_") + _PathFromContent.string();
			UpperCaseA(UpperCase);
			CodeLine += UpperCase;

			WriteCodeA(fp, CodeLine);
			WriteBracketOpenA(fp);

			CodeLine = string(PresetStr::EnumClass);
			CodeLine += "Idx";
			WriteCodeA(fp, CodeLine);
			WriteBracketOpenA(fp);

			//for문으로 순회를 돌아주면서 enum에 값을 추가해준다.
			for (const auto& iter : umapFile)
			{
				size_t size = iter.second.size();
				for (size_t i = 0; i < size; ++i)
				{
					//파일명을 대문자로 변환한 뒤 소괄호()와 . 문자를 제거한다.
					CodeLine = iter.second[i].filename().string();
					UpperCaseA(CodeLine);
					std::replace_if(CodeLine.begin(), CodeLine.end(),
						[](char c)->bool { return ('(' == c || ')' == c || '.' == c); },
						'_'
					);
					CodeLine += ',';
					WriteCodeA(fp, CodeLine);
				}
			}

			//enum에는 순회를 돌 떄를 생각해서 END 값도 추가
			WriteCodeA(fp, "END");
			//순회 다 돌았으면 괄호를 닫아준다.
			WriteBracketCloseA(fp, true);


			//다시 순회를 돌면서 이번엔 배열 형태로 작성한다.
			CodeLine = string(PresetStr::ConstexprStringView);
			CodeLine += "arrKey";
			CodeLine += "[] = ";
			WriteCodeA(fp, CodeLine);
			WriteBracketOpenA(fp);

			auto iter = umapFile.begin();
			const auto iterEnd = umapFile.end();
			while (iter != iterEnd)
			{
				size_t size = iter->second.size();
				for (size_t i = 0; i < size; ++i)
				{
					CodeLine = "\"";
					CodeLine += iter->second[i].string();
					CodeLine += "\"";
					std::replace(CodeLine.begin(), CodeLine.end(), '\\', '/');

					//순회가 끝나지 않았을 경우에는 쉽표를 추가
					//if (i + 1 != size || std::next(iter) != iterEnd)
					CodeLine += ',';

					WriteCodeA(fp, CodeLine);
				}

				++iter;
			}

			
			WriteBracketCloseA(fp, true);
			WriteBracketCloseAllA(fp);


			

			fp.close();
		}

			//		int iComp = curPath.parent_path().compare(prevPath);

			//		//들어온 경로가 지난 경로와 다를 경우
			//		if (0 != iComp)
			//		{
			//			//타고들어온 폴더의 갯수를 계산한다.
			//			int numDirCur = 0;
			//			for (filesystem::path::iterator iter = curPath.begin(); iter != curPath.end(); ++iter)
			//			{
			//				++numDirCur;
			//			}

			//			//폴더를 하나 빠져나왔거나 폴더 위치만 변경되었을 경우(a/b -> a/c)
			//			//괄호를 닫아 준다.
			//			if (numDirPrev >= numDirCur)
			//			{
			//				WriteBracketCloseA(fp);
			//			}

			//			//두 조건 모두 같을 때(==) 조건도 확인해 주어야 한다.
			//			//같을 떄는 괄호를 닫은 다음, 다시 열어야 주어야 하기 때문.

			//			//폴더를 하나 더 파고 들어왔거나, 폴더 위치만 변경되었을 경우(a/b -> a/c)
			//			//부모 폴더명으로 네임스페이스 이름을 생성한다.
			//			//폴더만 바뀌었을 경우에는 괄호를 닫아 주어야 한다.
			//			if (numDirPrev <= numDirCur)
			//			{
			//				WriteCodeA(fp);
			//				CodeLine = PresetStr::NameSpace;

			//				string UpperCase = curPath.parent_path().filename().string();
			//				transform(UpperCase.begin(), UpperCase.end(), UpperCase.begin(), ::toupper);

			//				CodeLine += UpperCase;							
			//				WriteCodeA(fp, CodeLine);
			//				WriteBracketOpenA(fp);
			//			}


			//			//prevPath를 변경.
			//			prevPath = curPath.parent_path();
			//			numDirPrev = numDirCur;
			//		}


			//		//키값을 추가한다. 이 때, 키(변수명)은 파일명으로만 해놓고,
			//		//실제 Value 값은 자신의 리소스 타입 이후로 해준다.
			//		//ex. Texture/SCUnit/Marine.png -> SCUnit/Marine.png
			//		CodeLine = PresetStr::ConstexprStringView;

			//		string UpperCase = curPath.filename().replace_extension("").string();
			//		transform(UpperCase.begin(), UpperCase.end(), UpperCase.begin(), ::toupper);
			//		replace_if(UpperCase.begin(), UpperCase.end(),
			//			[](char c) { return ('(' == c || ')' == c); }
			//		, '_');
			//		CodeLine += UpperCase;
			//		CodeLine += PresetStr::EqualDoubleQuotation;

			//		const filesystem::path& ValuePath = curPath.lexically_relative(FullPath);
			//		int PathCount = 0;
			//		//for (auto iter = ValuePath.begin(); iter != ValuePath.end(); ++iter)
			//		//{
			//		//	if (0 < PathCount)
			//		//	{
			//		//		CodeLine += "\\";
			//		//	}

			//		//	CodeLine += iter->string();
			//		//}
			//		string PathString = ValuePath.string();
			//		std::replace(PathString.begin(), PathString.end(), '\\', '/');
			//		CodeLine += PathString;
			//		CodeLine += "\";";

			//		WriteCodeA(fp, CodeLine);

			//	}

			//}


			////재귀 순회가 끝났을 경우 중괄호를 닫아준다.
			//WriteBracketCloseAllA(fp);
			//}
		catch (const std::filesystem::filesystem_error& error)
		{
			MessageBoxA(nullptr, error.what(), NULL, MB_OK);
			throw(std::runtime_error("Process Terminated by error."));
		}

		//작성이 완료되었으면 파일을 저장한다.
		fp.close();
	}


	
}


