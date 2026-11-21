#include "tabela_ll1.h"

namespace {

constexpr EntradaTabela p(int numeroDaProducao) {
    return {TipoEntrada::PRODUCAO, numeroDaProducao};
}

constexpr EntradaTabela ERR = {TipoEntrada::ERRO, 0};

// Tabela LL(1) M[A, a].
//
// Linhas na ordem de NaoTerminal; colunas na ordem de TipoToken.
// p(n) = usar a produção n;  ERR = célula vazia (erro).
//
// Como a tabela foi montada (Louden, cap. 4):
//   para cada produção A → α:
//     - para cada terminal a em FIRST(α), M[A, a] = A → α
//     - se ε está em FIRST(α), para cada b em FOLLOW(A), M[A, b] = A → α
//   as células que sobram são erros.
const EntradaTabela TABELA[QUANTIDADE_DE_NAO_TERMINAIS][QUANTIDADE_DE_TERMINAIS] = {
    //           num    +      -      *      /      ^      (      )      $
    /* E  */  { p(1),  ERR,   ERR,   ERR,   ERR,   ERR,   p(1),  ERR,   ERR   },
    /* E' */  { ERR,   p(2),  p(3),  ERR,   ERR,   ERR,   ERR,   p(4),  p(4)  },
    /* T  */  { p(5),  ERR,   ERR,   ERR,   ERR,   ERR,   p(5),  ERR,   ERR   },
    /* T' */  { ERR,   p(8),  p(8),  p(6),  p(7),  ERR,   ERR,   p(8),  p(8)  },
    /* F  */  { p(9),  ERR,   ERR,   ERR,   ERR,   ERR,   p(9),  ERR,   ERR   },
    /* F' */  { ERR,   p(11), p(11), p(11), p(11), p(10), ERR,   p(11), p(11) },
    /* P  */  { p(13), ERR,   ERR,   ERR,   ERR,   ERR,   p(12), ERR,   ERR   },
};

} // namespace

EntradaTabela consultarTabela(NaoTerminal naoTerminal, TipoToken lookahead) {
    return TABELA[static_cast<int>(naoTerminal)][static_cast<int>(lookahead)];
}
