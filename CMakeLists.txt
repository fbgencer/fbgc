cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(fbgc VERSION 0.1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")



add_subdirectory("src")
add_subdirectory("cmodules")

add_executable (fbgc fbgc.c)

target_link_libraries (fbgc LINK_PUBLIC fbgc_libs fbgc_cmodules)

#set (CMAKE_CXX_FLAGS "-Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")
#set (CMAKE_C_FLAGS "-Os -fdata-sections -ffunction-sections -flto -fno-optimize-sibling-calls -fno-omit-frame-pointer")
#DELETE THE LAST UNDERSCORE TO OPEN LOG PRINTING FOR A SPECIFIC H FILE

target_compile_definitions(fbgc_libs PUBLIC -DLOG_CLASS_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_CMODULE_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_COMPLEX_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_CSTRUCT_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_DOUBLE_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_FBGC_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_FIELD_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_FUN_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_GRAMMAR_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_INT_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_INTERPRETER_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_LINKEDLIST_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_LOGIC_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_MATRIX_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_MAP_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_MEMORY)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_OPERATOR_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_PARSER_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_RANGE_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_RELEXER_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_STR_OBJECT_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_SYMBOL_TABLE_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_TOKENS_)
target_compile_definitions(fbgc_libs PUBLIC -DLOG_TUPLE_OBJECT_)

