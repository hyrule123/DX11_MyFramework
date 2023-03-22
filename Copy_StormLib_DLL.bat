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
