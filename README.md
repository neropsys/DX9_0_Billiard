# This is my first readme file
# 0. 프로젝트 파일의 sln(프로젝트 솔루션 파일)을 실행해서 프로젝트 업그레이드를 해준다.
# 1. bonbong에서 주는 winzip파일 압축을 풀고 Include와 Lib폴더를 프로젝트 폴더에 넣는다
# 2. 프로젝트 속성에 들어가서 링커->고급-> 이미지에 안전한 예외 처리기 포함을 "아니오" 로 설정
# 3. VC++ 디렉터리->포함(Include) 디렉터리를 1번의 Include폴더로 설정
# 4. VC++ 디렉터리->라이브러리(Library) 디렉터리를 1번의 Lib폴더로 설정
# 5. 포함/라이브러리 디렉터리를 보면 대충 
# C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Include;$(IncludePath)
# 이렇게 되어있을 것이다. 순서를 바꿔준다
# 
# $(IncludePath);C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Include
# $(LibraryPath);C:\Users\user\Documents\GitHub\proj3_VirtualBilliard\Lib
# 이렇게

# 6.d3dUtility.h 가장 상단에 #pragma warning(disable:2026) 추가
# 7.d3dUtility.h 가장 하단에 const float INFINITY = FLT_MAX; 를 const float INF = FLT_MAX; 로 수정
# 8.d3dUtility.cpp에서 INFINITY를 모두 INF로 수정