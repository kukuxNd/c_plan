cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(CoreGame VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 添加可执行文件
add_executable(core_begin
    src/a/core_begin.cpp
)
# 检测是否为 Windows 系统
if(WIN32)
    # 为所有目标定义 _WIN32 宏
    add_definitions(-D_WIN32)
    
    # 或者只为特定目标定义
    target_compile_definitions(core_begin PRIVATE _WIN32)
endif()

function(add_exc_one exe_name src_dir)
    add_executable(${exe_name} src/${src_dir}/${exe_name}.cpp)
    target_include_directories(${exe_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endfunction()

function(add_exc_glob exe_name src_dir)
    file(GLOB SOURCES src/${src_dir}/*.cpp)
    add_executable(${exe_name} ${SOURCES})
    target_include_directories(${exe_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endfunction()

set(c_exe 
    c1 
    c2 
    c3
    )

foreach(exe_name IN LISTS c_exe)
    add_exc_one(${exe_name} c)
endforeach()

set(b_exe 
    b1 
    )

foreach(exe_name IN LISTS b_exe)
    add_exc_glob(${exe_name} b/b1)
endforeach()

# 如果您有任何依赖库，可以在这里添加
# find_package(some_package REQUIRED)
# target_link_libraries(core_game PRIVATE some_library)

# 包含目录

# 如果您有资源文件，可以添加到编译中
# add_custom_command(TARGET core_game POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:core_game>/resources
# )
