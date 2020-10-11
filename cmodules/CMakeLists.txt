FILE(GLOB csources *.c)
add_library(fbgc_cmodules ${csources})


target_include_directories(fbgc_cmodules PUBLIC .)
target_link_libraries(fbgc_cmodules m)