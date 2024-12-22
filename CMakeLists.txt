cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(deep_rock_hack VERSION 1.0.0)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/minhook_debug_x64/lib/minhook) #设置minhook路径
link_directories(dx11_imgui_lib/lib)
include_directories(dx11_imgui_lib/include)
find_package(minhook)
include_directories(${CMAKE_CURRENT_LIST_DIR}/imgui_d11)
add_library(${PROJECT_NAME} SHARED dllmain.cpp
engine.cpp hook.cpp pch.cpp utils.cpp
DeepRockFuncHook.cpp  )


target_link_libraries(${PROJECT_NAME} d3d11.lib minhook::minhook dx11_imgui_lib.lib)



add_executable(Launcher appMain.cpp )