#include "tabela_ll1.h"

namespace {

constexpr EntradaTabela p(int numeroDaProducao) {
    return {TipoEntrada::PRODUCAO, numeroDaProducao};
}

constexpr EntradaTabela e(int codigoDoErro) {
    return {TipoEntrada::ERRO, codigoDoErro};
}

// Tabela LL(1) M[A, a].
//
// Linhas na ordem de NaoTerminal; colunas na ordem de TipoToken.
// p(n) = usar a produção n;  e(n) = erro de código n.
//
// Como a tabela foi montada (Louden, cap. 4):
//   para cada produção A → α:
//     - para cada terminal a em FIRST(α), M[A, a] = A → α
//     - se ε está em FIRST(α), para cada b em FOLLOW(A), M[A, b] = A → α
//   as células que sobram são erros.
const EntradaTabela TABELA[QUANTIDADE_DE_NAO_TERMINAIS][QUANTIDADE_DE_TERMINAIS] = {
    //           num    +      -      *      /      ^      (      )      $
    /* E  */  { p(1),  e(1),  e(1),  e(1),  e(1),  e(1),  p(1),  e(1),  e(2)  },
    /* E' */  { e(3),  p(2),  p(3),  e(6),  e(6),  e(6),  e(3),  p(4),  p(4)  },
    /* T  */  { p(5),  e(1),  e(1),  e(1),  e(1),  e(1),  p(5),  e(1),  e(2)  },
    /* T' */  { e(3),  p(8),  p(8),  p(6),  p(7),  e(6),  e(3),  p(8),  p(8)  },
    /* F  */  { p(9),  e(1),  e(1),  e(1),  e(1),  e(1),  p(9),  e(1),  e(2)  },
    /* F' */  { e(3),  p(11), p(11), p(11), p(11), p(10), e(3),  p(11), p(11) },
    /* P  */  { p(13), e(1),  e(1),  e(1),  e(1),  e(1),  p(12), e(1),  e(2)  },
};

} // namespace

EntradaTabela consultarTabela(NaoTerminal naoTerminal, TipoToken lookahead) {
    return TABELA[static_cast<int>(naoTerminal)][static_cast<int>(lookahead)];
}

std::string descricaoDoErro(int codigo) {
    switch (codigo) {
        case ERRO_FALTA_OPERANDO:
            return "falta um operando: esperava um número ou '('";
        case ERRO_FIM_INESPERADO:
            return "a expressão terminou antes da hora: esperava um número ou '('";
        case ERRO_FALTA_OPERADOR:
            return "falta um operador entre dois operandos";
        case ERRO_PARENTESE_NAO_FECHADO:
            return "parêntese não fechado: esperava ')'";
        case ERRO_SIMBOLO_EXCEDENTE:
            return "símbolo excedente: a expressão já estava completa (')' sem '(' correspondente?)";
        case ERRO_OPERADOR_FORA_DE_LUGAR:
            return "operador fora de lugar";
        default:
            return "erro desconhecido";
    }
}
