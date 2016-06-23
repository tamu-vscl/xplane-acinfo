cmake_minimum_required(VERSION 2.8.12)
project(acinfo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
list(APPEND CMAKE_MODULE_PATH ${acinfo_SOURCE_DIR}/cmake_modules)
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()

# Version
set(XPA_MAJOR_VERSION 0)
set(XPA_MINOR_VERSION 1)
set(XPA_PATCH_VERSION 0)
set(XPA_VERSION ${XPA_MAJOR_VERSION}.${XPA_MINOR_VERSION}.${XPA_PATCH_VERSION})

# Output configuration.
set(acinfo_RUNTIME_OUTPUT_DIRECTORY ${acinfo_SOURCE_DIR}/bin CACHE PATH
    "Target for the binaries")
set(acinfo_LIBRARY_OUTPUT_DIRECTORY ${acinfo_SOURCE_DIR}/lib
    CACHE PATH "Target for the libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${acinfo_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${acinfo_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${acinfo_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_PREFIX /opt/xplane10/Resources/plugins/acinfo)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# X-Plane SDK defines.
if(APPLE)
  add_definitions(-DAPL=1)
  add_definitions(-DIBM=0)
  add_definitions(-DLIN=0)
elseif(WIN32)
  add_definitions(-DAPL=0)
  add_definitions(-DIBM=1)
  add_definitions(-DLIN=0)
elseif(UNIX)
  add_definitions(-DAPL=0)
  add_definitions(-DIBM=0)
  add_definitions(-DLIN=1)
endif()
# SDK version.
add_definitions(-DXPLM=1)
add_definitions(-DXPLM200)
add_definitions(-DXPLM210)
add_definitions(-shared -rdynamic -nodefaultlibs -undefined_warning)
# Code version.
add_definitions(-D_VERSION="${XPA_VERSION}")

# C configuration.
add_definitions(-D_DEFAULT_SOURCE)
add_definitions(-D_POSIX_C_SOURCE=2)
add_definitions(-D_FORTIFY_SOURCE=2)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra -Wfloat-equal")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wundef -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wstrict-overflow=5")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wwrite-strings -Waggregate-return")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-default -Wswitch-enum")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunreachable-code")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fstack-protector -funwind-tables")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fasynchronous-unwind-tables")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0 -O2 -std=c99 -m64")

# Variables
set(LIB_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)
set(XP_DIR ${EXTERN_DIR}/sdk)
set(XP_HEADER_DIR ${XP_DIR}/CHeaders)
if(APPLE)
  set(XP_LIB_DIR ${XP_DIR}/Libraries/Mac)
  find_library(XPLM_LIB XPLM
    PATHS ${XP_LIB_DIR})
  find_library(XPWidgets_LIB XPWidgets
    PATHS ${XP_LIB_DIR})
elseif(WIN32)
  set(XP_LIB_DIR ${XP_DIR}/Libraries/Win)
elseif(UNIX)
  # There is no library linking for linux
  set(XP_LIB_DIR "")
  # Instead, we have to add some extra command line options
  add_definitions(-shared -rdynamic -nodefaultlibs -undefined_warning)
endif()

link_directories(
  ${LIB_DIR}
  ${XP_LIB_DIR}
)

include_directories(
  ${HEADER_DIR}
  ${XP_HEADER_DIR}/XPLM
  ${XP_HEADER_DIR}/Widgets
)

add_subdirectory(src)

# Install fat plugin stuff.
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/64" DESTINATION
  "${CMAKE_INSTALL_PREFIX}")

# Build RPM
set(CPACK_PACKAGE_VERSION ${XPA_VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "xplane-acinfo")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Joshua Harris <joshua.a.harris@tamu.edu>")
set(CPACK_PACKAGE_VENDOR "Vehicle Systems & Control Laboratory")
set(CPACK_RPM_PACKAGE_LICENSE "ISC License")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
include(CPack)
