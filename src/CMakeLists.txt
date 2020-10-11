FILE(GLOB csources *.c)
add_library(fbgc_libs ${csources})

target_include_directories(fbgc_libs PUBLIC . ../include )