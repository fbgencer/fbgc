cmake_minimum_required(VERSION 3.10)


# set the project name and version
project(fbgc_test VERSION 0.0.0)

# -- Install configuration: ""
# -- Up-to-date: /usr/local/lib/libfbgc.so.0.0.1
# -- Up-to-date: /usr/local/lib/libfbgc.so.1
# -- Up-to-date: /usr/local/lib/libfbgc.so
# -- Installing: /usr/local/include/fbgc.h
# -- Installing: /usr/local/share/pkgconfig/fbgc.pc


#target_link_libraries(fbgc_test fbgc)
add_executable (fbgc_test test.c)

#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

find_library(fbgc_location NAMES libfbgc.so)
message(STATUS ${fbgc_location})

add_library(fbgc SHARED IMPORTED)
set_target_properties(fbgc PROPERTIES IMPORTED_LOCATION ${fbgc_location})

target_link_libraries( fbgc_test fbgc )

target_include_directories(fbgc_test PUBLIC .)
target_include_directories(fbgc_test PUBLIC ../include)

#target_include_directories(fbgc_test PUBLIC acutest/include)

#-DTEST_NO_MAIN