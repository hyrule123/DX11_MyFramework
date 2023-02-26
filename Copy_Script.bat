:: *.h 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\Script\*.h" ".\External\Include\Script\"

:: 참조용 파일 읽기 전용으로 변경
attrib +r ".\External\Include\Script\*"