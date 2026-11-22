// Analisador sintático LL(1) para expressões aritméticas.
//
// Uso:
//   parser_ll1 "2.5 * (3 + 4) ^ 2"        analisa a expressão dada
//   parser_ll1                            pede a expressão no terminal
//   parser_ll1 --dot arvore.dot "1 + 2"   também grava a árvore em .dot (Graphviz)

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "analisador_lexico.h"
#include "analisador_sintatico.h"
#include "arvore_sintatica.h"
#include "exportar_dot.h"

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

// Executa as três etapas pedidas para a expressão.
// Devolve true se a expressão foi aceita.
bool processarExpressao(const std::string& expressao, const std::string& arquivoDot) {
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

    // iv. Árvore de análise sintática
    imprimirTitulo("3. Árvore de análise sintática");
    imprimirArvore(*sintatico.arvore, std::cout);

    if (!arquivoDot.empty()) {
        if (exportarArvoreDot(*sintatico.arvore, arquivoDot)) {
            std::cout << "\nÁrvore gravada em " << arquivoDot << "\n";
        } else {
            std::cout << "\nNão foi possível gravar " << arquivoDot << "\n";
        }
    }
    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    configurarTerminalParaUtf8();

    std::string expressao;
    std::string arquivoDot;

    // Tudo o que não for a opção --dot faz parte da expressão,
    // então  parser_ll1 2 + 3  também funciona.
    for (int i = 1; i < argc; i++) {
        const std::string argumento = argv[i];
        if (argumento == "--dot") {
            if (i + 1 >= argc) {
                std::cout << "A opção --dot precisa do nome do arquivo.\n";
                return 2;
            }
            arquivoDot = argv[++i];
        } else {
            if (!expressao.empty()) expressao += " ";
            expressao += argumento;
        }
    }

    // Sem expressão nos argumentos: pede uma no terminal.
    if (expressao.empty()) {
        std::cout << "Digite a expressão: ";
        std::getline(std::cin, expressao);
    }

    return processarExpressao(expressao, arquivoDot) ? 0 : 1;
}
