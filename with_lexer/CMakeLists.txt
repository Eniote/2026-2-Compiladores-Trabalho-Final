add_executable(with_lexer main.cpp
        ../common_lib/myUtil.cpp
        ../common_lib/myUtil.h)

target_link_libraries(with_lexer PRIVATE common_lib)