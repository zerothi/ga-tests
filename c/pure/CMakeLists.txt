cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Check we don't build in the top-level dir
get_filename_component(_srcdir "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
get_filename_component(_bindir "${CMAKE_CURRENT_BINARY_DIR}" REALPATH)

if("${_srcdir}" STREQUAL "${_bindir}")
  message(FATAL_ERROR
    "It is not allowed to configure and build this project from its source folder. Please, create a \
separate build directory and invoke CMake from that directory.")
endif()


list(APPEND CMAKE_MESSAGE_CONTEXT pure)

project(pure
  LANGUAGES C
  DESCRIPTION "Simple, pure C compilation"
  )

# Follow GNU conventions for installing directories
# This will use the bin/ include/ etc. folder structure
include(GNUInstallDirs)
include(CMakePrintHelpers)


# Define options for this project
option(BUILD_SHARED_LIBS "Build using shared libraries" FALSE)
if( BUILD_SHARED_LIBS )
  include(CheckPIESupported)
  check_pie_supported()

  message(STATUS "Enabling position independent code")
elseif( BUILD_SHARED_LIBS )
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Building shared libs" FORCE)
endif()


option(PURE_WITH_BLAS "Enable code with BLAS calls" FALSE)
option(PURE_INSTALL "Enable install calls" ${PROJECT_IS_TOP_LEVEL})


# Print options
cmake_print_variables(PROJECT_NAME)
cmake_print_variables(BUILD_SHARED_LIBS)
cmake_print_variables(PURE_WITH_BLAS)
cmake_print_variables(PURE_INSTALL)
cmake_print_variables(CMAKE_CROSSCOMPILING)
cmake_print_variables(CMAKE_HOST_SYSTEM_NAME)
cmake_print_variables(CMAKE_SYSTEM_NAME)
cmake_print_variables(CMAKE_HOST_SYSTEM_PROCESSOR)
cmake_print_variables(CMAKE_SYSTEM_PROCESSOR)


if( PURE_WITH_BLAS )
  find_package(BLAS REQUIRED)
endif()


add_library(${PROJECT_NAME}.pure
  pure_lib.c
  )
set_target_properties(${PROJECT_NAME}.pure
  PROPERTIES
  OUTPUT_NAME pure
  )

target_compile_definitions(${PROJECT_NAME}.pure
  PUBLIC
  $<$<BOOL:${PURE_WITH_BLASMPI}>:PURE__BLAS>
  )
target_link_libraries(${PROJECT_NAME}.pure
  INTERFACE
  $<$<BOOL:${PURE_WITH_BLAS}>:BLAS::BLAS>
  )
target_include_directories(${PROJECT_NAME}.pure
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# Add executables
add_executable(${PROJECT_NAME}.pure.x
  pure.c
  )
set_target_properties(${PROJECT_NAME}.pure.x
  PROPERTIES
  OUTPUT_NAME pure.x
  )

target_link_libraries(${PROJECT_NAME}.pure.x
  PRIVATE
  ${PROJECT_NAME}.pure
  )


if( PURE_INSTALL )
  install(
    TARGETS ${PROJECT_NAME}.pure
    RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
  install(
    TARGETS ${PROJECT_NAME}.pure.x
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
endif()

list(POP_FRONT CMAKE_MESSAGE_CONTEXT)
