set Mode=%1
if "%Mode%"=="" set Mode=Debug
echo Running %0... 
echo Mode: %Mode%

:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\UtilLib_DLL\json\*.h" ".\External\Include\UtilLib_DLL\json\"

:: *.h 읽기 전용으로 변경
attrib +r ".\External\Include\UtilLib_DLL\json\*.h"

if exist ".\External\Library\UtilLib_DLL\UtilLib_DLL_%Mode%.dll" (
xcopy /d /s /y /i ".\External\Library\UtilLib_DLL\UtilLib_DLL_%Mode%.dll" ".\OutputFile\Bin_%Mode%\"
xcopy /d /s /y /i ".\External\Library\UtilLib_DLL\UtilLib_DLL_%Mode%.dll" ".\"
)
