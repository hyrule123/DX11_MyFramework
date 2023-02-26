:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\UtilDLL\json-cpp\*.h" ".\External\Include\UtilDLL\"

if exist ".\External\Library\UtilDLL\UtilDLL_Debug.dll" (
xcopy /d /s /y /i ".\External\Library\UtilDLL\UtilDLL_Debug.dll" ".\OutputFile\Bin_Debug\"
)

if exist ".\External\Library\UtilDLL\UtilDLL.dll" (
xcopy /d /s /y /i ".\External\Library\UtilDLL\UtilDLL.dll" ".\OutputFile\Bin\"
)


:: *.h 읽기 전용으로 변경
attrib +r ".\External\Include\UtilDLL\*.h"