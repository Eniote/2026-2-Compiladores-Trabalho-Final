#ifndef PARSER_LL1_ARVORE_SINTATICA_H
#define PARSER_LL1_ARVORE_SINTATICA_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>

// Nó da árvore de análise sintática.
//   - Nó interno: um não-terminal (E, E', T, ...), com um filho para cada
//     símbolo do lado direito da produção usada.
//   - Folha: um terminal casado com a entrada (num, +, (, ...) ou ε.
struct NoArvore {
    std::string rotulo;   // "E", "E'", "num", "+", "ε", ...
    std::string lexema;   // texto da entrada, preenchido quando o terminal é casado
    std::vector<std::unique_ptr<NoArvore>> filhos;

    // Cria um filho no fim da lista e devolve um ponteiro para ele.
    NoArvore* adicionarFilho(const std::string& rotuloDoFilho);
};

// Imprime a árvore no terminal, um nó por linha, usando ├── e └──.
void imprimirArvore(const NoArvore& raiz, std::ostream& saida);

#endif // PARSER_LL1_ARVORE_SINTATICA_H
