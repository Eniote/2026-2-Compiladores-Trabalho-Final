flex_target(WithLexer lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

add_executable(with_lexer main.cpp ${FLEX_WithLexer_OUTPUTS})

target_link_libraries(with_lexer PRIVATE common_lib)