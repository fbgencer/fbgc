cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(fbgc VERSION 0.0.1 DESCRIPTION "fbgc")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")

include(GNUInstallDirs)

FILE(GLOB csrc src/*.c)
FILE(GLOB ccmodules cmodules/*.c)
add_library(fbgc SHARED ${csrc} ${ccmodules})

target_include_directories(fbgc PUBLIC .)
target_include_directories(fbgc PUBLIC include)
target_include_directories(fbgc PUBLIC cmodules)
target_link_libraries(fbgc m) #For math

set_target_properties(fbgc PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/fbgc.h)

configure_file(fbgc.pc.in fbgc.pc @ONLY)

install(TARGETS fbgc 
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/fbgc.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


# add_subdirectory("src")
# add_subdirectory("cmodules")

# add_executable (fbgc fbgc.c)

# target_link_libraries (fbgc LINK_PUBLIC fbgc fbgc_cmodules)

# target_include_directories(fbgc PUBLIC .)

#set (CMAKE_CXX_FLAGS "-Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")
#set (CMAKE_C_FLAGS "-Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")

#DELETE THE LAST UNDERSCORE TO OPEN LOG PRINTING FOR A SPECIFIC H FILE
target_compile_definitions(fbgc PUBLIC -DLOG_CLASS_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_CMODULE_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_COMPLEX_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_CSTRUCT_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_DOUBLE_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_FBGC_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_FIELD_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_FUN_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_GRAMMAR_)
target_compile_definitions(fbgc PUBLIC -DLOG_INT_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_INTERPRETER)
target_compile_definitions(fbgc PUBLIC -DLOG_LINKEDLIST_)
target_compile_definitions(fbgc PUBLIC -DLOG_LOGIC_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_MATRIX_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_MAP_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_MEMORY_)
target_compile_definitions(fbgc PUBLIC -DLOG_OPERATOR_)
target_compile_definitions(fbgc PUBLIC -DLOG_PARSER)
target_compile_definitions(fbgc PUBLIC -DLOG_RANGE_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_RELEXER_)
target_compile_definitions(fbgc PUBLIC -DLOG_STR_OBJECT_)
target_compile_definitions(fbgc PUBLIC -DLOG_SYMBOL_TABLE_)
target_compile_definitions(fbgc PUBLIC -DLOG_TOKENS_)
target_compile_definitions(fbgc PUBLIC -DLOG_TUPLE_OBJECT_)


add_executable(fbgcex fbgc.c)
target_link_libraries (fbgcex LINK_PUBLIC fbgc)
#target_include_directories(fbgcex PUBLIC .)
