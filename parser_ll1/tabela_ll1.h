#ifndef PARSER_LL1_TABELA_LL1_H
#define PARSER_LL1_TABELA_LL1_H

#include <string>

#include "gramatica.h"
#include "token.h"

// Cada célula M[A, a] da tabela contém uma produção ou um erro.
enum class TipoEntrada { PRODUCAO, ERRO };

struct EntradaTabela {
    TipoEntrada tipo;
    int numero;  // número da produção (1 a 13); 0 nas células de erro
};

// Consulta M[naoTerminal, lookahead].
EntradaTabela consultarTabela(NaoTerminal naoTerminal, TipoToken lookahead);

#endif // PARSER_LL1_TABELA_LL1_H
