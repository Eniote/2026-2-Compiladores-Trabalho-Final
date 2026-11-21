add_executable(parser_ll1
        main.cpp
        analisador_lexico.cpp
        gramatica.cpp
        tabela_ll1.cpp)

# Os fontes têm caracteres UTF-8 (→, ε, ├, acentos).
# O GCC/MinGW já assume UTF-8; o MSVC precisa ser avisado.
if(MSVC)
    target_compile_options(parser_ll1 PRIVATE /utf-8)
endif()
