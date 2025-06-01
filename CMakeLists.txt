cmake_minimum_required(VERSION 3.0.0)
project(lvgl_demo VERSION 0.1.0)

include(CTest)
enable_testing()

# 添加宏定义
add_definitions(-DUSE_SDL)

# 添加头文件目录
include_directories(src)

# 添加子库编译
add_subdirectory("lvgl")
add_subdirectory("lv_drivers")

# 添加源文件(递归包含)
FILE(GLOB_RECURSE src_source ./src/*.c)

# 编译可执行文件
add_executable(${PROJECT_NAME} ${src_source})

# 链接库
target_link_libraries(${PROJECT_NAME} lvgl)
target_link_libraries(${PROJECT_NAME} lv_drivers)
target_link_libraries(${PROJECT_NAME} mingw32)
target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2)

# 拷贝SDL2.dll 文件到 build目录
file(COPY SDL2.dll DESTINATION ../build)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
