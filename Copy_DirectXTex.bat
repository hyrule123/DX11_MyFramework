:: *.h, inl 파일 복사(라이브러리 참조용)
xcopy /d /s /y /i /r /exclude:exclude_list.txt ".\Project\DirectXTex\DirectXTex.h" ".\External\Include\DirectXTex\"

::헤더 파일 읽기 전용으로 변경
attrib +r ".\External\Include\DirectXTex\*.h"

xcopy /d /s /y /i ".\Project\DirectXTex\DirectXTex.inl" ".\External\Include\DirectXTex\"