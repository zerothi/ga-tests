cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

list(APPEND CMAKE_MESSAGE_CONTEXT pure)

project(pure
  LANGUAGES Fortran
  DESCRIPTION "Simple, pure fortran compilation"
  )

# Follow GNU conventions for installing directories
# This will use the bin/ include/ etc. folder structure
include(GNUInstallDirs)


# Define options for this project
option(BUILD_SHARED_LIBS "Build using shared libraries" FALSE)
option(PURE_SHARED_LIBS "Build as a shared library" "${BUILD_SHARED_LIBS}")
if( PURE_SHARED_LIBS )
  include(CheckPIESupported)
  check_pie_supported()

  message(STATUS "Enabling position independent code")
  # Ensure BUILD_SHARED_LIBS is set
  set(BUILD_SHARED_LIBS ON CACHE BOOL "Building shared libs" FORCE)
elseif( BUILD_SHARED_LIBS )
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Building shared libs" FORCE)
endif()


option(PURE_WITH_BLAS "Enable code with BLAS calls" FALSE)
option(PURE_INSTALL "Enable install calls" ${PROJECT_IS_TOP_LEVEL})


if( PURE_WITH_BLAS )
  find_package(BLAS REQUIRED)
endif()


add_library(${PROJECT_NAME}.pure
  pure_lib.F90
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

# Add executables
add_executable(${PROJECT_NAME}.pure.x
  pure.F90
  )
target_link_libraries(${PROJECT_NAME}.pure.x
  PRIVATE
  ${PROJECT_NAME}.pure
  )

set_target_properties(${PROJECT_NAME}.pure.x
  PROPERTIES
  OUTPUT_NAME pure.x
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

