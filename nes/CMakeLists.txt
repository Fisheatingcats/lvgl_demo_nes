cmake_minimum_required(VERSION 3.10)
project(nes VERSION 1.0 LANGUAGES C)

# 模块配置选项
option(BUILD_SHARED_LIBS "Build as shared library" OFF)
set(INC_INSTALL_DIR "include" CACHE STRING "Installation directory for headers")

# 源文件收集（精确模式）
file(GLOB NES_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/nes_mapper/*.c"
)

# 创建库目标
add_library(nes ${NES_SOURCES})

# 头文件包含配置（关键修正）
target_include_directories(nes PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>  # 构建时路径
    $<INSTALL_INTERFACE:${INC_INSTALL_DIR}>            # 安装后路径
)

# 属性配置
set_target_properties(nes PROPERTIES
    C_VISIBILITIES_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# 链接依赖
target_link_libraries(nes PUBLIC 
    lvgl 
)
target_link_libraries(nes PUBLIC SDL2)
# 安装规则
install(TARGETS nes
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/inc/"
    DESTINATION "${INC_INSTALL_DIR}/nes"
    FILES_MATCHING PATTERN "*.h"
)