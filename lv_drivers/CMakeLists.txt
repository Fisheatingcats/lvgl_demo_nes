cmake_minimum_required(VERSION 3.12.4)

project(lv_drivers HOMEPAGE_URL https://github.com/lvgl/lv_drivers/)

# Option to build as shared library (as opposed to static), default: OFF
option(BUILD_SHARED_LIBS "Build shared as library (as opposed to static)" OFF)

file(GLOB_RECURSE SOURCES ./*.c)

if (BUILD_SHARED_LIBS)
  add_library(lv_drivers SHARED ${SOURCES})
else()
  add_library(lv_drivers STATIC ${SOURCES})
endif()

add_library(lvgl_drivers ALIAS lv_drivers)
add_library(lvgl::drivers ALIAS lv_drivers)

target_include_directories(lv_drivers SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig)
pkg_check_modules(PKG_WAYLAND wayland-client wayland-cursor wayland-protocols xkbcommon)
pkg_check_modules(PKG_LVGL lvgl)
target_link_libraries(lv_drivers PUBLIC lvgl ${PKG_WAYLAND_LIBRARIES})

if("${LIB_INSTALL_DIR}" STREQUAL "")
  set(LIB_INSTALL_DIR "lib")
endif()

if("${INC_INSTALL_DIR}" STREQUAL "")
  set(INC_INSTALL_DIR "include/lvgl/lv_drivers")
endif()

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/${INC_INSTALL_DIR}/"
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN ".git*" EXCLUDE
  PATTERN "CMakeFiles" EXCLUDE
  PATTERN "docs" EXCLUDE
  PATTERN "lib" EXCLUDE
  PATTERN "*.pc.in" EXCLUDE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PKG_LVGL_CFLAGS}")

# configure_file("${CMAKE_SOURCE_DIR}/lv-drivers.pc.in" lv-drivers.pc @ONLY)

install(
  FILES "${CMAKE_BINARY_DIR}/lv-drivers.pc"
  DESTINATION "${LIB_INSTALL_DIR}/pkgconfig/")

file(GLOB LV_DRIVERS_PUBLIC_HEADERS "${CMAKE_SOURCE_DIR}/lv_drv_conf.h")

target_include_directories(lv_drivers
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../  # 包含父目录
    ${CMAKE_BINARY_DIR}  # 包含构建目录
)

install(
    FILES 
    ${CMAKE_SOURCE_DIR}/lv_drv_conf.h  # 源目录头文件
    ${CMAKE_BINARY_DIR}/lv_drv_conf.h  # 生成的头文件
    DESTINATION "${INC_INSTALL_DIR}"
)

set_target_properties(
  lv_drivers
  PROPERTIES OUTPUT_NAME lv_drivers
             ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             PUBLIC_HEADER "${LV_DRIVERS_PUBLIC_HEADERS}")

install(
  TARGETS lv_drivers
  ARCHIVE DESTINATION "${LIB_INSTALL_DIR}"
  LIBRARY DESTINATION "${LIB_INSTALL_DIR}"
  RUNTIME DESTINATION "${LIB_INSTALL_DIR}"
  PUBLIC_HEADER DESTINATION "${INC_INSTALL_DIR}")
