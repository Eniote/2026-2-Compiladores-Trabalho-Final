#ifndef PARSER_LL1_GRAMATICA_H
#define PARSER_LL1_GRAMATICA_H

#include <string>
#include <vector>

#include "token.h"

// Gramática transformada (sem recursão à esquerda e fatorada à esquerda):
//
//    1. E  → T E'
//    2. E' → + T E'
//    3. E' → - T E'
//    4. E' → ε
//    5. T  → F T'
//    6. T' → * F T'
//    7. T' → / F T'
//    8. T' → ε
//    9. F  → P F'
//   10. F' → ^ F
//   11. F' → ε
//   12. P  → ( E )
//   13. P  → num
//
// Os terminais são os próprios tipos de token (TipoToken).

// A ordem dos valores é a mesma ordem das linhas da tabela LL(1).
enum class NaoTerminal { E, E_LINHA, T, T_LINHA, F, F_LINHA, P };

constexpr int QUANTIDADE_DE_NAO_TERMINAIS = 7;

// Um símbolo da gramática: ou é terminal, ou é não-terminal.
struct Simbolo {
    bool ehTerminal;
    TipoToken terminal;        // usado apenas quando ehTerminal == true
    NaoTerminal naoTerminal;   // usado apenas quando ehTerminal == false
};

Simbolo simboloTerminal(TipoToken terminal);
Simbolo simboloNaoTerminal(NaoTerminal naoTerminal);

std::string nomeDoNaoTerminal(NaoTerminal naoTerminal);  // "E", "E'", "T", ...
std::string nomeDoSimbolo(const Simbolo& simbolo);       // "E'", "+", "num", "$", ...

struct Producao {
    int numero;
    NaoTerminal ladoEsquerdo;
    std::vector<Simbolo> ladoDireito;  // lista vazia representa ε
};

constexpr int QUANTIDADE_DE_PRODUCOES = 13;

// Devolve a produção de número 1 a 13 (numeração acima).
const Producao& obterProducao(int numero);

// Texto da produção, ex.: "E' → + T E'" ou "E' → ε".
std::string producaoComoTexto(const Producao& producao);

#endif // PARSER_LL1_GRAMATICA_H
