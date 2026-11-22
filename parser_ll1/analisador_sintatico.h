#ifndef PARSER_LL1_ANALISADOR_SINTATICO_H
#define PARSER_LL1_ANALISADOR_SINTATICO_H

#include <ostream>
#include <string>
#include <vector>

#include "token.h"

// Uma linha da tabela de passos do parse.
struct PassoDoParse {
    int numero;
    std::string pilha;    // base ($) à esquerda, topo à direita
    std::string entrada;  // tokens ainda não consumidos, terminando em $
    std::string acao;     // produção usada, casamento, ACEITA ou ERRO
};

struct ResultadoSintatico {
    bool aceita;
    std::vector<PassoDoParse> passos;
    int codigoDoErro;                  // 0 quando a expressão é aceita
    std::string mensagemDeErro;
    int colunaDoErro;
};

// Analisador sintático LL(1) dirigido por tabela.
// Recebe os tokens produzidos pelo analisador léxico (terminando em FIM)
// e para no primeiro erro encontrado.
ResultadoSintatico analisarSintaticamente(const std::vector<Token>& tokens);

// Imprime os passos alinhados em colunas: Passo | Pilha | Entrada | Ação.
void imprimirPassos(const std::vector<PassoDoParse>& passos, std::ostream& saida);

#endif // PARSER_LL1_ANALISADOR_SINTATICO_H
