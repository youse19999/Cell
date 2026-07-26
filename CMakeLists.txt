# ---CMarker---
# Written by yousei saitou
# ---CMarker---

#----------------------------------------------
# Variable for specifying all library versions
#----------------------------------------------
set(GOOGLE_TEST_VERSION "v1.17.0")

#
# Behavior:Setting up Windows static library distribution
# 
# Goal:Runtime components are statically provided within Windows.
#
# Result:The runtime library is statically embedded within the executable file.
#
macro(marker_make_static_win_lib)
	set(CompilerFlags
	  CMAKE_CXX_FLAGS
	  CMAKE_CXX_FLAGS_DEBUG
	  CMAKE_CXX_FLAGS_RELEASE
	  CMAKE_C_FLAGS
	  CMAKE_C_FLAGS_DEBUG
	  CMAKE_C_FLAGS_RELEASE
	)

	foreach(CompilerFlag IN LISTS CompilerFlags)
		if(DEFINED ${CompilerFlag})
			string(REPLACE "/MD" "/MT" UpdatedFlag "${${CompilerFlag}}")
			set(${CompilerFlag} "${UpdatedFlag}" CACHE STRING "MSVC Dynamic to Static Runtime" FORCE)
		endif()
	endforeach()
endmacro()

#
# Behavior:Run FetchContent to retrieve the required library versions for the test environment based on the team's current version.
# 
# Goal:Prevent environment contamination caused by the unauthorized acquisition of libraries within the team.
#
# Result:All libraries will be available in the specified versions.
#
macro(marker_fetch_component)
	include(FetchContent)

	FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY https://github.com/google/googletest.git
	  GIT_TAG        ${GOOGLE_TEST_VERSION}
	)

	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(googletest)
endmacro()

# ---CMarker---
# Written by yousei saitou
# ---CMarker---






# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.10)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

marker_make_static_win_lib()
marker_fetch_component()

project ("CMakeProject4")

# サブプロジェクトを含めます。
add_subdirectory ("CMakeProject4")
