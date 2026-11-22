#ifndef PARSER_LL1_TOKEN_H
#define PARSER_LL1_TOKEN_H

#include <string>

// Tipos de token da linguagem de expressões.
//
// A ordem dos valores NÃO é arbitrária: ela é a mesma ordem das colunas
// da tabela LL(1) (ver tabela_ll1.cpp). Por isso o valor do enum pode ser
// usado diretamente como índice de coluna.
enum class TipoToken {
    NUM,        // num : 12, 3.14
    MAIS,       // +
    MENOS,      // -
    VEZES,      // *
    DIVIDIDO,   // /
    POTENCIA,   // ^
    ABRE_PAR,   // (
    FECHA_PAR,  // )
    FIM         // $ : marca o fim da entrada
};

constexpr int QUANTIDADE_DE_TERMINAIS = 9;

struct Token {
    TipoToken tipo;
    std::string lexema;  // texto exatamente como apareceu na expressão
    int coluna;          // posição (a partir de 1) do primeiro caractere do lexema
};

// Nome do token, usado na listagem da análise léxica (ex.: "NUM", "MAIS").
std::string nomeDoToken(TipoToken tipo);

// Símbolo terminal correspondente na gramática (ex.: "num", "+", "$").
std::string simboloDoTerminal(TipoToken tipo);

#endif // PARSER_LL1_TOKEN_H
