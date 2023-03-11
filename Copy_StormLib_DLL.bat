chcp 65001

:: Copy *.h file(for library reference) 테스트
xcopy /d /s /y /i /r /exclude:exclude_list_stormlib.txt "./Project/StormLib_DLL/SRC/*.h" "./External/Include/StormLib_DLL/"

if exist "./External/Library/StormLib_DLL/StormLib_DLL_Debug.dll" (
xcopy /d /s /y /i "./External/Library/StormLib_DLL/StormLib_DLL_Debug.dll" "./OutputFile/Bin_Debug/"
)

if exist "./External/Library/StormLib_DLL/StormLib_DLL_Release.dll" (
xcopy /d /s /y /i "./External/Library/StormLib_DLL/StormLib_DLL_Release.dll" "./OutputFile/Bin_Release/"
)


:: Add read-only attributes to *.h files
attrib +r "./External/Include/StormLib_DLL/*.h"
