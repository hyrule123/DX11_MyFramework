
:: 형태는 main()의 argc, argv과 동일함. 
:: 0번 파라미터에는 파일 경로가 들어옴
:: 1번 파라미터에는 실행 메시지가 들어옴
:: 1번 메시지에는 빌드 이벤트에서 빌드 모드에 따라 Debug/Release 메시지가 들어오도록 해놓음
:: -> cd $(SolutionDir) "$(SolutionDir)Copy_Engine.bat" $(Configuration)
:: 아래 코드는 해당 메시지를 참조해서 경로를 생성하는 코드임. 
:: %(변수)% = 문자열
set Mode=%1
if "%Mode%"=="" set Mode=Debug
echo Running %0... 
echo Mode: %Mode%

:: Copy *.h file(for library reference) 테스트
xcopy /d /s /y /i /r /exclude:exclude_list_stormlib.txt ".\Project\stormLib_DLL\SRC\*.h" ".\External\Include\stormLib_DLL\"

::CodeGenerator.exe에서도 사용하므로 파일을 복사해 준다.
if exist ".\External\Library/stormLib_DLL/stormLib_DLL_%Mode%.dll" (
xcopy /d /s /y /i ".\External\Library\stormLib_DLL\stormLib_DLL_%Mode%.dll" ".\OutputFile\Bin_%Mode%\"
)

:: Add read-only attributes to *.h files
attrib +r ".\External\Include\stormLib_DLL\*.h"
