cmake_minimum_required(VERSION 3.16)
project(806_HeatGeodesics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Standalone app: depend on libigl core + GLFW viewer only (not the tutorial target).
set(LIBIGL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBIGL_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
set(LIBIGL_GLFW ON CACHE BOOL "" FORCE)
set(LIBIGL_OPENGL ON CACHE BOOL "" FORCE)

set(LIBIGL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../libigl/libigl" CACHE PATH "Path to libigl source tree")
if(NOT EXISTS "${LIBIGL_ROOT}/CMakeLists.txt")
  message(FATAL_ERROR "libigl not found at ${LIBIGL_ROOT}. Set LIBIGL_ROOT to your libigl checkout.")
endif()

add_subdirectory("${LIBIGL_ROOT}" "${CMAKE_BINARY_DIR}/libigl")

# Default mesh shipped alongside the app (from libigl-tutorial-data).
set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(DEFAULT_MESH "${DATA_DIR}/beetle.off")
file(MAKE_DIRECTORY "${DATA_DIR}")
if(NOT EXISTS "${DEFAULT_MESH}")
  message(STATUS "Downloading default mesh to ${DEFAULT_MESH}")
  file(DOWNLOAD
    "https://raw.githubusercontent.com/libigl/libigl-tutorial-data/644dd4104843b6d736745d9dafbd70bf8d175648/beetle.off"
    "${DEFAULT_MESH}"
    SHOW_PROGRESS
    TLS_VERIFY ON
  )
endif()

file(GLOB SRCFILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(806_HeatGeodesics ${SRCFILES})

target_compile_definitions(806_HeatGeodesics PRIVATE "DEFAULT_MESH=\"${DEFAULT_MESH}\"")

target_link_libraries(806_HeatGeodesics PRIVATE
  igl::core
  igl::glfw
)

# Keep a copy next to the executable for runs launched from the build tree.
add_custom_command(TARGET 806_HeatGeodesics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:806_HeatGeodesics>/data"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DEFAULT_MESH}"
    "$<TARGET_FILE_DIR:806_HeatGeodesics>/data/beetle.off"
  COMMENT "Copying data/beetle.off next to executable"
)
