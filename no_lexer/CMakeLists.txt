add_executable(no_lexer main.cpp
        ../common_lib/myUtil.cpp
        ../common_lib/myUtil.h)

target_link_libraries(no_lexer PRIVATE common_lib)