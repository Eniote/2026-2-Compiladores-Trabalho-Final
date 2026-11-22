#ifndef PARSER_LL1_TABELA_LL1_H
#define PARSER_LL1_TABELA_LL1_H

#include <string>

#include "gramatica.h"
#include "token.h"

// Cada célula M[A, a] da tabela contém uma produção ou um código de erro.
enum class TipoEntrada { PRODUCAO, ERRO };

struct EntradaTabela {
    TipoEntrada tipo;
    int numero;  // número da produção (1 a 13) ou código do erro (1 a 6)
};

// Códigos de erro sintático.
// e1, e2, e3 e e6 aparecem nas células da tabela (topo da pilha é não-terminal).
// e4 e e5 acontecem quando o topo da pilha é um terminal diferente do lookahead.
constexpr int ERRO_FALTA_OPERANDO          = 1;  // e1
constexpr int ERRO_FIM_INESPERADO          = 2;  // e2
constexpr int ERRO_FALTA_OPERADOR          = 3;  // e3
constexpr int ERRO_PARENTESE_NAO_FECHADO   = 4;  // e4
constexpr int ERRO_SIMBOLO_EXCEDENTE       = 5;  // e5
constexpr int ERRO_OPERADOR_FORA_DE_LUGAR  = 6;  // e6

// Consulta M[naoTerminal, lookahead].
EntradaTabela consultarTabela(NaoTerminal naoTerminal, TipoToken lookahead);

// Texto explicativo de cada código de erro.
std::string descricaoDoErro(int codigo);

#endif // PARSER_LL1_TABELA_LL1_H
