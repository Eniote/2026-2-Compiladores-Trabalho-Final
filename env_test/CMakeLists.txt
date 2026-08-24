# Generate lexer.cpp from lexer.l
flex_target(EnvLexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

# Compile the test application
add_executable(env_test main.cpp ${FLEX_EnvLexer_OUTPUTS}
        ../common_lib/myUtil.cpp
        ../common_lib/myUtil.h)

target_link_libraries(env_test PRIVATE common_lib)