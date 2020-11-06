cmake_minimum_required(VERSION 3.17.0)


project(string2maplib VERSION 0.1.0 DESCRIPTION "string2map library")

add_library(string2maplib SHARED src/string2map.cpp)

set_target_properties(string2maplib PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER src/string2map.h)

target_include_directories(string2maplib PRIVATE .)
target_compile_features(string2maplib PUBLIC cxx_std_17)

install(TARGETS string2maplib
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
