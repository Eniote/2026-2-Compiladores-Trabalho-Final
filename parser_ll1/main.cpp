// Analisador sintático LL(1) para expressões aritméticas.
//
// Uso:
//   parser_ll1 "2.5 * (3 + 4) ^ 2"        analisa a expressão dada
//   parser_ll1                            pede a expressão no terminal

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "analisador_lexico.h"
#include "analisador_sintatico.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

void configurarTerminalParaUtf8() {
#ifdef _WIN32
    // Sem isto o console do Windows mostra lixo no lugar de →, ε, ├, └ e acentos.
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void imprimirTitulo(const std::string& titulo) {
    std::cout << "\n== " << titulo << " ==\n\n";
}

void imprimirTokens(const std::vector<Token>& tokens) {
    std::cout << "Lexema     Token      Coluna\n";
    std::cout << "---------- ---------- ------\n";
    for (const Token& token : tokens) {
        std::string lexema = token.lexema;
        std::string tipo = nomeDoToken(token.tipo);
        lexema.resize(std::max<size_t>(lexema.size(), 10), ' ');
        tipo.resize(std::max<size_t>(tipo.size(), 10), ' ');
        std::cout << lexema << " " << tipo << " " << token.coluna << "\n";
    }
}

void imprimirErro(const std::string& tipoDoErro, int coluna, const std::string& mensagem) {
    std::cout << "\n" << tipoDoErro << " na coluna " << coluna << ": " << mensagem << "\n";
}

// Executa as etapas pedidas para a expressão.
// Devolve true se a expressão foi aceita.
bool processarExpressao(const std::string& expressao) {
    std::cout << "Expressão: " << expressao << "\n";

    // i. Análise léxica
    imprimirTitulo("1. Análise léxica");
    const ResultadoLexico lexico = analisarLexicamente(expressao);
    imprimirTokens(lexico.tokens);
    if (!lexico.sucesso) {
        imprimirErro("Erro léxico", lexico.colunaDoErro, lexico.mensagemDeErro);
        return false;
    }

    // ii e iii. Análise sintática LL(1) mostrando pilha, entrada e ação
    imprimirTitulo("2. Análise sintática LL(1)");
    const ResultadoSintatico sintatico = analisarSintaticamente(lexico.tokens);
    imprimirPassos(sintatico.passos, std::cout);
    if (!sintatico.aceita) {
        imprimirErro("Erro sintático", sintatico.colunaDoErro, sintatico.mensagemDeErro);
        return false;
    }
    std::cout << "\nResultado: expressão ACEITA.\n";
    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    configurarTerminalParaUtf8();

    std::string expressao;

    // Todos os argumentos formam a expressão,
    // então  parser_ll1 2 + 3  também funciona.
    for (int i = 1; i < argc; i++) {
        if (!expressao.empty()) expressao += " ";
        expressao += argv[i];
    }

    // Sem expressão nos argumentos: pede uma no terminal.
    if (expressao.empty()) {
        std::cout << "Digite a expressão: ";
        std::getline(std::cin, expressao);
    }

    return processarExpressao(expressao) ? 0 : 1;
}
