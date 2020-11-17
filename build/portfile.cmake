if(NOT VCPKG_TARGET_IS_WINDOWS)
    message(FATAL_ERROR "\n${PORT} does not support your system, only Windows for now. Please open a ticket issue on github.com/microsoft/vcpkg if necessary\n")
endif()

set(VERSION 1.1.0)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SiddiqSoft/string2map
    SHA512 1
    REF v1.1.0
    HEAD_REF master
)

if (VCPKG_TARGET_ARCHITECTURE MATCHES "x64")
    set(BUILD_ARCH "x64")
else()
    message(FATAL_ERROR "Unsupported architecture: ${VCPKG_TARGET_ARCHITECTURE}")
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()

file(INSTALL ${SOURCE_PATH}/COPYING DESTINATION ${CURRENT_PACKAGES_DIR}/share/string2map RENAME copyright)