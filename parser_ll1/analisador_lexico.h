#ifndef PARSER_LL1_ANALISADOR_LEXICO_H
#define PARSER_LL1_ANALISADOR_LEXICO_H

#include <string>
#include <vector>

#include "token.h"

struct ResultadoLexico {
    bool sucesso;
    // Tokens reconhecidos. Se sucesso, a lista sempre termina com o token FIM ($).
    // Se houve erro, contém os tokens lidos até o ponto do erro.
    std::vector<Token> tokens;
    std::string mensagemDeErro;
    int colunaDoErro;
};

// Quebra a expressão em tokens.
// Espaços, tabulações e quebras de linha são ignorados.
// Para no primeiro erro léxico encontrado.
ResultadoLexico analisarLexicamente(const std::string& expressao);

#endif // PARSER_LL1_ANALISADOR_LEXICO_H
