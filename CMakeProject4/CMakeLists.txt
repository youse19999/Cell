# CMakeList.txt : CMakeProject4 の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (CMakeProject4 "CMakeProject4.cpp" "CMakeProject4.h" "Include/loader/tiny_gltf.cc")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject4 PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(CMakeProject4 PRIVATE Include Include/loader)
target_link_libraries(CMakeProject4 PUBLIC d3d11 d3dcompiler)

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
